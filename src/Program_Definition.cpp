#include "Program_Definition.h"

ProgramTransmitter::ProgramTransmitter(char* title,
    MovingCommandResolver* commandResolver, MessageRenderer* messageRenderer,
    RF24Tranceiver* tranceiver, uint64_t address) {
  _title = title;
  if (address != 0) {
    _rf24Address = address;
  }
  _rf24Tranceiver = tranceiver;
  _messageSender = tranceiver;
  _messageRenderer = messageRenderer;
  _commandResolver = commandResolver;
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

void ProgramTransmitter::set(MovingCommandResolver* commandResolver) {
  _commandResolver = commandResolver;
}

int ProgramTransmitter::check(void* inputData) {
  JoystickAction* action = (JoystickAction*) inputData;

  _counter.ordinalNumber += 1;
  _counter.packetLossTotal += 1;

  if (action == NULL) {
    return -1;
  }

  MovingCommand movingCommand;

  if (_commandResolver != NULL) {
    _commandResolver->resolve(&movingCommand, action, 3);
  }

  #if JOYSTICK_CHECKING_CHANGE
  if (!action->isChanged()) {
    return 1;
  }
  #endif

  if (_messageSender != NULL) {
    MessagePacket packet(action, &movingCommand);
    bool ok = _messageSender->write(&packet);
    if (ok) {
      _counter.continualLossCount = 0;
      _counter.packetLossTotal -= 1;
    } else {
      _counter.continualLossCount += 1;
    }
  }

  #if MULTIPLE_SENDERS_SUPPORTED
  MessagePacket packet2(action, &movingCommand);
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
    _messageRenderer->render(action, &movingCommand, &_counter);
  }

  _counter.adjust();

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
    Serial.print('#'), Serial.print(_counter.ordinalNumber), Serial.print("->"), Serial.print(index), Serial.print(": ");
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

char* ProgramTransmitter::getTitle() {
  return _title;
}

int ProgramTransmitter::begin() {
  return _rf24Tranceiver->begin(RF24_TX, _rf24Address);
}

int ProgramTransmitter::close() {
  _rf24Tranceiver->reset(RF24_TX);
  return 0;
}

ProgramReceiver::ProgramReceiver(char* title,
    RF24Tranceiver* tranceiver, uint64_t address) {
  _title = title;
  if (address != 0) {
    _rf24Address = address;
  }
  _rf24Tranceiver = tranceiver;
}

char* ProgramReceiver::getTitle() {
  return _title;
}

int ProgramReceiver::begin() {
  return _rf24Tranceiver->begin(RF24_RX, _rf24Address);
}

int ProgramReceiver::check(void* action) {
  return _rf24Tranceiver->check();
}

int ProgramReceiver::close() {
  _rf24Tranceiver->reset(RF24_RX);
  return 0;
}
