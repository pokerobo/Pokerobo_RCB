#include "RF24_Tranceiver.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#ifndef __DEBUG_LOG_RF24_TRANCEIVER__
#define __DEBUG_LOG_RF24_TRANCEIVER__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_RF24_TRANCEIVER__

#ifndef __TRACE_LOG_RF24_TRANCEIVER__
#define __TRACE_LOG_RF24_TRANCEIVER__ __RUNNING_LOG_ENABLED__
#endif//__TRACE_LOG_RF24_TRANCEIVER__

#ifndef __OPTIMIZING_DYNAMIC_MEMORY__
#define __OPTIMIZING_DYNAMIC_MEMORY__     1
#endif

#ifndef RF24_TWO_WAY_ENABLED
#define RF24_TWO_WAY_ENABLED              1
#endif//RF24_TWO_WAY_ENABLED

#ifndef RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING
#define RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING 1
#endif//RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING

#ifndef RF24_RECEIVER_DISCONTINUITY_MAX
#define RF24_RECEIVER_DISCONTINUITY_MAX  5000
#endif//RF24_RECEIVER_DISCONTINUITY_MAX

#ifndef RF24_PRIMARY_PIN_CE
#define RF24_PRIMARY_PIN_CE  9
#endif//RF24_PRIMARY_PIN_CE

#ifndef RF24_PRIMARY_PIN_CSN
#define RF24_PRIMARY_PIN_CSN 10
#endif//RF24_PRIMARY_PIN_CSN

#ifndef RF24_SECONDARY_RADIO_ENABLED
#define RF24_SECONDARY_RADIO_ENABLED  false
#endif//RF24_SECONDARY_RADIO_ENABLED

#ifndef RF24_SECONDARY_PIN_CE
#define RF24_SECONDARY_PIN_CE           A3
#endif//RF24_SECONDARY_PIN_CE

#ifndef RF24_SECONDARY_PIN_CSN
#define RF24_SECONDARY_PIN_CSN          A2
#endif//RF24_SECONDARY_PIN_CSN

RF24 radio(RF24_PRIMARY_PIN_CE, RF24_PRIMARY_PIN_CSN);

RF24* getPrimaryRadio() {
  return &radio;
}

#if RF24_SECONDARY_RADIO_ENABLED
RF24 radio2(RF24_SECONDARY_PIN_CE, RF24_SECONDARY_PIN_CSN);

RF24* getSecondaryRadio() {
  return &radio2;
}
#else
RF24* getSecondaryRadio() {
  return &radio;
}
#endif

//-------------------------------------------------------------------------------------------------

int RF24Tranceiver::begin(tranceiver_t mode, uint64_t address) {
  if (mode == RF24_TX) {
    return RF24Transmitter::begin(address, getPrimaryRadio());
  }
  if (mode == RF24_RX) {
    return RF24Receiver::begin(address, getSecondaryRadio());
  }
  return -1;
}

void RF24Tranceiver::reset(tranceiver_t mode) {
  if (mode == RF24_TX) {
    RF24Transmitter::reset();
    return;
  }
  if (mode == RF24_RX) {
    RF24Receiver::reset();
    return;
  }
}

void reset_(RF24* _tranceiver) {
  _tranceiver->powerDown();
  delay(100);
  _tranceiver->powerUp();
}

//-------------------------------------------------------------------------------------------------

int RF24Transmitter::begin(uint64_t address, void* radio) {
  if (radio == NULL) {
    return -1;
  }
  _transmitter = radio;
  RF24* _tranceiver = (RF24*)_transmitter;
  _tranceiver->begin();
  _tranceiver->openWritingPipe(address);
  _tranceiver->openReadingPipe(1, 0xFFFFFFFFLL ^ address);
  #if RF24_TWO_WAY_ENABLED
  _tranceiver->startListening();
  #endif
  return 0;
}

void RF24Transmitter::reset() {
  reset_((RF24*)_transmitter);
}

bool RF24Transmitter::write(MessagePacket* packet) {
  if (packet == NULL) {
    _status = MESSAGE_NULL;
    return false;
  }
  uint8_t len = packet->length();
  uint8_t msg[len] = {};
  packet->serialize(msg, len);
  return write(msg, len);
}

bool RF24Transmitter::write(const void* buf, uint8_t len) {
  if (_transmitter == NULL) {
    _status = TRANSMITTER_NULL;
    return false;
  }
  RF24* _tranceiver = (RF24*)_transmitter;

  #if RF24_TWO_WAY_ENABLED
  #if RF24_TRANCEIVER_DELAY_BEFORE_STOP_LISTENING
  delay(RF24_TRANCEIVER_DELAY_BEFORE_STOP_LISTENING);
  #endif//RF24_TRANCEIVER_DELAY_BEFORE_STOP_LISTENING
  _tranceiver->stopListening();
  #if RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING
  delay(RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING);
  #endif//RF24_TRANCEIVER_DELAY_AFTER_STOP_LISTENING
  #endif

  bool result = _tranceiver->write(buf, len);

  #if RF24_TWO_WAY_ENABLED
  #if RF24_TRANCEIVER_DELAY_BEFORE_START_LISTENING
  delay(RF24_TRANCEIVER_DELAY_BEFORE_START_LISTENING);
  #endif//RF24_TRANCEIVER_DELAY_BEFORE_START_LISTENING
  _tranceiver->startListening();
  #if RF24_TRANCEIVER_DELAY_AFTER_START_LISTENING
  delay(RF24_TRANCEIVER_DELAY_AFTER_START_LISTENING);
  #endif//RF24_TRANCEIVER_DELAY_AFTER_START_LISTENING
  #endif

  if (!result) {
    #if __DEBUG_LOG_RF24_TRANCEIVER__
    if (_tranceiver->isChipConnected()) {
      Serial.print("RF24Transmitter"), Serial.print("::"), Serial.print("write"),
        Serial.print('-'), Serial.print('>'),
        Serial.println("error");
    } else {
      Serial.print("RF24Transmitter"), Serial.print("::"), Serial.print("write"),
        Serial.print(':'), Serial.print(' '),
        Serial.println("broken");
    }
    #endif
  } else {
    #if __TRACE_LOG_RF24_TRANCEIVER__
    Serial.print("RF24Transmitter"), Serial.print("::"), Serial.print("write"),
        Serial.print('-'), Serial.print('>'),
        Serial.println("ok");
    #endif
  }
  _status = result ? ACK_OK : ACK_FAILED;
  return result;
}

rf24_tx_status_t RF24Transmitter::getStatus() {
  return _status;
}

//-------------------------------------------------------------------------------------------------

int RF24Receiver::begin(uint64_t address, void* radio) {
  if (radio == NULL) {
    return -1;
  }
  _receiver = radio;
  RF24* _tranceiver = (RF24*)_receiver;
  _tranceiver->begin();
  _tranceiver->openWritingPipe(0xFFFFFFFFLL ^ address);
  _tranceiver->openReadingPipe(1, address);
  _tranceiver->startListening();
  return 0;
}

void RF24Receiver::reset() {
  // reset the _receiver
  reset_((RF24*)_receiver);
  // reset the _counter
  _counter.baselineNumber = 0;
  _counter.ordinalNumber = 0;
  _counter.packetLossTotal = 0;
  // clear the _messageRenderer
  if (_messageRenderer != NULL) {
    _messageRenderer->clear();
  }
  #if MULTIPLE_RENDERERS_SUPPORTED
  for(int i=0; i<_messageRenderersTotal; i++) {
    MessageRenderer* messageRenderer = _messageRenderers[i];
    if (messageRenderer != NULL) {
      messageRenderer->clear();
    }
  }
  #endif
}

bool RF24Receiver::available() {
  RF24* _tranceiver = (RF24*)_receiver;

  // resets the IRQ pin to HIGH
  bool tx_ds, tx_df, rx_dr;
  _tranceiver->whatHappened(tx_ds, tx_df, rx_dr);

  // returned data should now be reliable
  bool status = _tranceiver->available();

  if (status) {
    _discontinuityCount = 0;
    return status;
  }
  _discontinuityCount++;

  if (!status && _messageRenderer != NULL) {
    #if __OPTIMIZING_DYNAMIC_MEMORY__
    char info[14] = { 0 };
    if (_tranceiver->isChipConnected()) {
      if (_discontinuityCount > RF24_RECEIVER_DISCONTINUITY_MAX) {
        info[ 0] = 'L';
        info[ 1] = 'i';
        info[ 2] = 's';
        info[ 3] = 't';
        info[ 4] = 'e';
        info[ 5] = 'n';
        info[ 6] = 'n';
        info[ 7] = 'i';
        info[ 8] = 'n';
        info[ 9] = 'g';
        info[10] = '.';
        info[11] = '.';
        info[12] = '.';
        info[13] = '\0';
        _messageRenderer->splash(info, 5);
      }
    } else {
      info[ 0] = 'C';
      info[ 1] = 'o';
      info[ 2] = 'n';
      info[ 3] = 'n';
      info[ 4] = 'e';
      info[ 5] = 'c';
      info[ 6] = 't';
      info[ 7] = 'i';
      info[ 8] = 'n';
      info[ 9] = 'g';
      info[10] = '.';
      info[11] = '.';
      info[12] = '.';
      info[13] = '\0';
      _messageRenderer->splash(info, 5);
    }
    #else
    if (_tranceiver->isChipConnected()) {
      if (_discontinuityCount > RF24_RECEIVER_DISCONTINUITY_MAX) {
        char info[14] = { 'L', 'i', 's', 't', 'e', 'n', 'n', 'i', 'n', 'g', '.', '.', '.', '\0' };
        _messageRenderer->splash(info, 5);
      }
    } else {
      char info[14] = { 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'n', 'g', '.', '.', '.', '\0' };
      _messageRenderer->splash(info, 5);
    }
    #endif
  }
  return status;
}

int RF24Receiver::check() {
  if (_receiver == NULL) {
    return -1;
  }

  if (!available()) {
    return 0;
  }

  RF24* _tranceiver = (RF24*)_receiver;
  uint8_t msg[_messageSerializer->getSize()] = {0};
  _tranceiver->read(&msg, sizeof(msg));

  return _messageSerializer->decode(msg, this);
}

int RF24Receiver::process(JoystickAction* action, MessageInterface* commandPacket) {
  action->setSource(RX_MSG);

  uint32_t count = action->getExtras();

  if (_counter.ordinalNumber == 0) {
    _counter.baselineNumber = count;
    _counter.packetLossTotal = 0;
  } else {
    if (count < _counter.ordinalNumber + 1) {
      _counter.baselineNumber = count;
      _counter.packetLossTotal = 0;
    } else if (count == _counter.ordinalNumber + 1) {
    } else if (count > _counter.ordinalNumber + 1) {
      _counter.packetLossTotal += count - _counter.ordinalNumber - 1;
    }
  }
  _counter.ordinalNumber = count;

  if (_messageProcessor != NULL) {
    _messageProcessor->process(action, commandPacket);
  }

  if (_messageRenderer != NULL) {
    _messageRenderer->render(action, commandPacket, &_counter);
  }

  #if MULTIPLE_RENDERERS_SUPPORTED
  int8_t countNulls = 0, sumFails = 0, sumOk = 0;
  for(int i=0; i<_messageRenderersTotal; i++) {
    int8_t status = invoke(_messageRenderers[i], i+1, action, commandPacket, &_counter);
    if (status > 0) {
      sumOk += status;
    } else if (status < 0) {
      sumFails += status;
    } else {
      countNulls++;
    }
  }
  #endif
}

void RF24Receiver::set(MessageProcessor* messageProcessor) {
  _messageProcessor = messageProcessor;
}

void RF24Receiver::set(MessageSerializer* messageSerializer) {
  _messageSerializer = messageSerializer;
}

void RF24Receiver::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

#if MULTIPLE_RENDERERS_SUPPORTED
bool RF24Receiver::add(MessageRenderer* messageRenderer) {
  if (messageRenderer == NULL) {
    return false;
  }
  if (_messageRenderersTotal > MESSAGE_RENDERERS_LIMIT) {
    return false;
  }
  #if __STRICT_MODE__
  for(int i=0; i<_messageRenderersTotal; i++) {
    if (_messageRenderers[i] == messageRenderer) {
      return false;
    }
  }
  #endif
  _messageRenderers[_messageRenderersTotal++] = messageRenderer;
  return true;
}
#endif

#if MULTIPLE_RENDERERS_SUPPORTED
byte RF24Receiver::invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message,
      MessageInterface* commandPacket, TransmissionCounter* counter) {
  if (messageRenderer != NULL) {
    uint8_t code = 1 << index;
    bool ok = messageRenderer->render(message, commandPacket, counter);
    #if __DEBUG_LOG_RF24_TRANCEIVER__
    Serial.print('#'), Serial.print(message->getExtras()), Serial.print("->"), Serial.print(index), Serial.print(": ");
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
#endif
