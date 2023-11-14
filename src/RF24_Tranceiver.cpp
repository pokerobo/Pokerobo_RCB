#include "RF24_Tranceiver.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#ifndef __DEBUG_LOG_RF24_TRANCEIVER__
#define __DEBUG_LOG_RF24_TRANCEIVER__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_RF24_TRANCEIVER__

#if __PLATFORM_TYPE__ == __PLATFORM_MEGA2560__
#ifndef PIN_CE
#define PIN_CE  48
#endif

#ifndef PIN_CSN
#define PIN_CSN 49
#endif
#endif//__PLATFORM_TYPE__

#ifndef PIN_CE
#define PIN_CE  9
#endif

#ifndef PIN_CSN
#define PIN_CSN 10
#endif

RF24 radio(PIN_CE, PIN_CSN);

int RF24Tranceiver::begin(tranceiver_t mode, uint64_t address) {
  if (mode == RF24_TX) {
    return RF24Transmitter::begin(&radio, address);
  }
  if (mode == RF24_RX) {
    return RF24Receiver::begin(&radio, address);
  }
  return -1;
}

void RF24Tranceiver::reset(tranceiver_t mode) {
  if (mode == RF24_TX) {
    return RF24Transmitter::reset();
  }
  if (mode == RF24_RX) {
    return RF24Receiver::reset();
  }
}

int RF24Transmitter::begin(void* radio, uint64_t address) {
  if (radio == NULL) {
    return -1;
  }
  _transmitter = radio;
  RF24* _tranceiver = (RF24*)_transmitter;
  _tranceiver->begin();
  _tranceiver->openWritingPipe(address);
  _tranceiver->stopListening();
}

void RF24Transmitter::reset() {
  RF24* _tranceiver = (RF24*)_transmitter;
  _tranceiver->powerDown();
  delay(100);
  _tranceiver->powerUp();
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
  bool result = _tranceiver->write(buf, len);
  _status = result ? ACK_OK : ACK_FAILED;
  return result;
}

rf24_tx_status_t RF24Transmitter::getStatus() {
  return _status;
}

int RF24Receiver::begin(void* radio, uint64_t address) {
  if (radio == NULL) {
    return -1;
  }
  _receiver = radio;
  RF24* _tranceiver = (RF24*)_receiver;
  _tranceiver->begin();
  _tranceiver->openReadingPipe(0, address);
  _tranceiver->startListening();
}

void RF24Receiver::reset() {
  // reset the _receiver
  RF24* _tranceiver = (RF24*)_receiver;
  _tranceiver->powerDown();
  delay(100);
  _tranceiver->powerUp();
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

  if (!status) {
    bool connected = _tranceiver->isChipConnected();
    if (connected) {
      if (_messageRenderer != NULL) {
#if __SPACE_SAVING_MODE__
        char info[19] = {
            ' ', ' ', ' ', ' ', ' ',
            'L', 'i', 's', 't', 'e', 'n', 'n', 'i', 'n', 'g', '.', '.', '.', '\0'
        };
#else
        char info[14] = { 'L', 'i', 's', 't', 'e', 'n', 'n', 'i', 'n', 'g', '.', '.', '.', '\0' };
#endif
        _messageRenderer->splash(info);
      }
    } else {
      if (_messageRenderer != NULL) {
#if __SPACE_SAVING_MODE__
        char info[19] = {
            ' ', ' ', ' ', ' ', ' ',
            'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'n', 'g', '.', '.', '.', '\0'
        };
#else
        char info[14] = { 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'n', 'g', '.', '.', '.', '\0' };
#endif
        _messageRenderer->splash(info);
      }
    }
  }

  return status;
}

int RF24Receiver::check() {
  if (_receiver == NULL) {
    return -1;
  }

  RF24* _tranceiver = (RF24*)_receiver;

  if (!available()) {
    return 0;
  }

  uint8_t msg[JoystickAction::messageSize] = {0};
  _tranceiver->read(&msg, sizeof(msg));

  uint16_t buttons;
  uint16_t jX, jY;
  uint32_t count;

  bool ok = decodeMessage(msg, MESSAGE_SIGNATURE, &buttons, &jX, &jY, &count);

#if __DEBUG_LOG_RF24_TRANCEIVER__
  char log[32] = { 0 };
  char fmt[12] = { '%', 'd', ',', '%', 'd', ',', '%', 'd', ',', '%', 'd', '\0' };
  sprintf(log, fmt, buttons, jX, jY, count);
  Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
      Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
  Serial.println();
#endif

  if (!ok) {
    return -1;
  }

  JoystickAction message(buttons, jX, jY, count);
  message.setSource(RX_MSG);

  SpeedPacket speedPacketInstance;
  SpeedPacket* speedPacket = NULL;
  if (_speedResolver != NULL) {
    speedPacket = &speedPacketInstance;
    _speedResolver->resolve(speedPacket, &message);
  }

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

  if (_messageRenderer != NULL) {
    _messageRenderer->render(&message, speedPacket, &_counter);
  }

#if MULTIPLE_RENDERERS_SUPPORTED
  int8_t countNulls = 0, sumFails = 0, sumOk = 0;
  for(int i=0; i<_messageRenderersTotal; i++) {
    int8_t status = invoke(_messageRenderers[i], i+1, &message, speedPacket, &_counter);
    if (status > 0) {
      sumOk += status;
    } else if (status < 0) {
      sumFails += status;
    } else {
      countNulls++;
    }
  }
#endif

  return 0;
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
      SpeedPacket* speedPacket, TransmissionCounter* counter) {
  if (messageRenderer != NULL) {
    uint8_t code = 1 << index;
    bool ok = messageRenderer->render(message, speedPacket, counter);
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

void RF24Receiver::set(SpeedResolver* speedResolver) {
  _speedResolver = speedResolver;
}
