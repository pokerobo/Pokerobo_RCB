#include "Program_Entrypoint.h"

ProgramTransmitter::ProgramTransmitter(char* title,
    CommandPacket* commandBuffer, CommandResolver* commandResolver, MessageRenderer* messageRenderer,
    RF24Tranceiver* tranceiver, uint8_t offsetAddress): ProgramSticker(title) {
  initialize(commandBuffer, commandResolver, messageRenderer, tranceiver, offsetAddress);
}

ProgramTransmitter::ProgramTransmitter(char* titles[PROGRAM_TITLE_PARTS],
    CommandPacket* commandBuffer, CommandResolver* commandResolver, MessageRenderer* messageRenderer,
    RF24Tranceiver* tranceiver, uint8_t offsetAddress): ProgramSticker(titles) {
  initialize(commandBuffer, commandResolver, messageRenderer, tranceiver, offsetAddress);
}

void ProgramTransmitter::initialize(CommandPacket* commandBuffer,
      CommandResolver* commandResolver, MessageRenderer* messageRenderer,
      RF24Tranceiver* tranceiver, uint8_t offsetAddress) {
  if (offsetAddress != 0) {
    _rf24Address = offsetAddress;
  }
  _rf24Tranceiver = tranceiver;
  _messageSender = tranceiver;
  _messageRenderer = messageRenderer;
  _commandResolver = commandResolver;
  _commandBuffer = commandBuffer;
}

void ProgramTransmitter::set(MessageSender* messageSender) {
  _messageSender = messageSender;
}

#if MULTIPLE_SENDERS_SUPPORTED
bool ProgramTransmitter::add(MessageSender* messageSender) {
  if (messageSender == NULL) {
    return false;
  }
  if (_messageSendersTotal > MESSAGE_EXCHANGE_MAX) {
    return false;
  }
  #if __STRICT_MODE__
  for(int i=0; i<_messageSendersTotal; i++) {
    if (_messageSenders[i] == messageSender) {
      return false;
    }
  }
  #endif
  _messageSenders[_messageSendersTotal++] = messageSender;
  return true;
}
#endif//MULTIPLE_SENDERS_SUPPORTED

void ProgramTransmitter::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

void ProgramTransmitter::set(CommandResolver* commandResolver) {
  _commandResolver = commandResolver;
}

void ProgramTransmitter::set(CommandPacket* commandBuffer) {
  _commandBuffer = commandBuffer;
}

bool ProgramTransmitter::hasCommandBuffer() {
  return _commandBuffer != NULL;
}

int ProgramTransmitter::check(void* inputData, void* command) {
  JoystickAction* action = (JoystickAction*) inputData;

  getTransmissionCounter()->beginTransmission();

  if (action == NULL) {
    return -1;
  }

  CommandPacket* commandPacket = NULL;
  if (_commandResolver != NULL) {
    if (!hasCommandBuffer()) {
      commandPacket = _commandResolver->create();
    } else {
      commandPacket = _commandBuffer;
    }
    _commandResolver->resolve(commandPacket, action);
  }

  #if JOYSTICK_CHECKING_CHANGE
  if (!action->isChanged()) {
    return 1;
  }
  #endif

  if (_messageSender != NULL) {
    MasterContext* context = NULL;
    #if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V2
    MasterContext contextPacket(getId(), false);
    context = &contextPacket;
    #endif
    MessagePacket packet(context, action, commandPacket);
    bool ok = _messageSender->write(&packet);
    if (ok) {
      getTransmissionCounter()->confirmTransmissionSuccess();
    } else {
      getTransmissionCounter()->confirmTransmissionFailure();
    }
  }

  #if MULTIPLE_SENDERS_SUPPORTED
  MessagePacket packet2(action, commandPacket);
  int8_t countNulls = 0, sumFails = 0, sumOk = 0;
  for(int i=0; i<_messageSendersTotal; i++) {
    int8_t status = invoke(_messageSenders[i], i+1, NULL, 0, &packet2);
    if (status > 0) {
      sumOk += status;
    } else if (status < 0) {
      sumFails += status;
    } else {
      countNulls++;
    }
  }
  #endif

  if (_messageRenderer != NULL) {
    _messageRenderer->render(action, commandPacket, getTransmissionCounter());
  }

  getTransmissionCounter()->adjust();

  if (_commandResolver != NULL) {
    if (!hasCommandBuffer()) {
      _commandResolver->release(commandPacket);
    }
  }

  return 0;
}

#if MULTIPLE_SENDERS_SUPPORTED
byte ProgramTransmitter::invoke(MessageSender* messageSender, uint8_t index, const void* buf, uint8_t len, MessagePacket* packet) {
  if (messageSender != NULL) {
    uint8_t code = 1 << index;

    bool ok = false;
    if (packet != NULL) {
      ok = messageSender->write(packet);
    } else {
      ok = messageSender->write(buf, len);
    }

    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print('#'), Serial.print(getTransmissionCounter()->ordinalNumber), Serial.print("->"), Serial.print(index), Serial.print(": ");
    if (ok) {
      Serial.println('v');
    } else {
      Serial.println('x');
    }
    #endif

    if (ok) {
      return code;
    } else {
      return -code;
    }
  }
  return 0;
}
#endif//MULTIPLE_SENDERS_SUPPORTED

uint8_t ProgramTransmitter::getId() {
  return 1;
}

int ProgramTransmitter::begin() {
  return _rf24Tranceiver->begin(RF24_TX,
      (_rf24Address == DEFAULT_OFFSET_ADDRESS) ? RF24_DEFAULT_ADDRESS : RF24_BASE_ADDRESS + _rf24Address);
}

int ProgramTransmitter::close() {
  if (isCounterBuiltin()) {
    setTransmissionCounter(NULL);
  } else {
    if (isCounterShared()) {
      getTransmissionCounter()->reset();
    }
  }
  _rf24Tranceiver->reset(RF24_TX);
  return 0;
}

//-------------------------------------------------------------------------------------------------

ProgramReceiver::ProgramReceiver(char* title,
    RF24Tranceiver* tranceiver, uint8_t offsetAddress): ProgramSticker(title) {
  initialize(tranceiver, offsetAddress);
}

ProgramReceiver::ProgramReceiver(char* titles[PROGRAM_TITLE_PARTS],
    RF24Tranceiver* tranceiver, uint8_t offsetAddress): ProgramSticker(titles) {
  initialize(tranceiver, offsetAddress);
}

void ProgramReceiver::initialize(RF24Tranceiver* tranceiver, uint8_t offsetAddress) {
  if (offsetAddress != 0) {
    _rf24Address = offsetAddress;
  }
  _rf24Tranceiver = tranceiver;
}

uint8_t ProgramReceiver::getId() {
  return 2;
}

int ProgramReceiver::begin() {
  return _rf24Tranceiver->begin(RF24_RX,
      (_rf24Address == DEFAULT_OFFSET_ADDRESS) ? RF24_DEFAULT_ADDRESS : RF24_BASE_ADDRESS + _rf24Address);
}

int ProgramReceiver::check(void* action, void* command) {
  return _rf24Tranceiver->check();
}

int ProgramReceiver::close() {
  _rf24Tranceiver->reset(RF24_RX);
  return 0;
}

//-------------------------------------------------------------------------------------------------

ProgramConfigForm::ProgramConfigForm(char* title,
    ConfigDisplayHandler *displayHandler): ProgramSticker(title) {
  _displayHandler = displayHandler;
}

ProgramConfigForm::ProgramConfigForm(char* titles[PROGRAM_TITLE_PARTS],
    ConfigDisplayHandler *displayHandler): ProgramSticker(titles) {
  _displayHandler = displayHandler;
}

uint8_t ProgramConfigForm::getId() {
  return 4;
}

int ProgramConfigForm::begin() {
  return 0;
}

int ProgramConfigForm::check(void* action, void* command) {
  return 0;
}

int ProgramConfigForm::close() {
  return 0;
}

//-------------------------------------------------------------------------------------------------

ProgramDeviceInfo::ProgramDeviceInfo(char* title,
    DeviceDisplayHandler *displayHandler): ProgramSticker(title) {
  initialize(displayHandler);
}

ProgramDeviceInfo::ProgramDeviceInfo(char* titles[PROGRAM_TITLE_PARTS],
    DeviceDisplayHandler *displayHandler): ProgramSticker(titles) {
  initialize(displayHandler);
}

void ProgramDeviceInfo::initialize(DeviceDisplayHandler *displayHandler) {
  _displayHandler = displayHandler;
}

uint8_t ProgramDeviceInfo::getId() {
  return 3;
}

int ProgramDeviceInfo::begin() {
  return 0;
}

int ProgramDeviceInfo::check(void* action, void* command) {
  _displayHandler->render((DeviceManifest*)NULL);
  return 0;
}

int ProgramDeviceInfo::close() {
  return 0;
}
