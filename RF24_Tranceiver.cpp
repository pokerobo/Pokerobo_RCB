#include "RF24_Tranceiver.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// #define PIN_CE  48
// #define PIN_CSN 49

#ifndef PIN_CE
#define PIN_CE  9
#endif

#ifndef PIN_CSN
#define PIN_CSN 10
#endif

const uint64_t address = 0xE8E8F0F0E1LL;

RF24 radio(PIN_CE, PIN_CSN);

int RF24Tranceiver::begin(tranceiver_t mode) {
  if (mode == TX) {
    return RF24Transmitter::begin(&radio);
  }
  if (mode == RX) {
    return RF24Receiver::begin(&radio);
  }
  return -1;
}

int RF24Transmitter::begin(void* radio) {
  if (radio == NULL) {
    return -1;
  }
  _transmitter = radio;
  RF24* _tranceiver = (RF24*)_transmitter;
  _tranceiver->begin();
  _tranceiver->openWritingPipe(address);
  _tranceiver->stopListening();
}

bool RF24Transmitter::write(MessagePacket* packet) {
  if (packet == NULL) {
    return false;
  }
  uint8_t len = packet->length();
  uint8_t msg[len] = {};
  packet->serialize(msg, len);
  return write(msg, len);
}

bool RF24Transmitter::write(const void* buf, uint8_t len) {
  if (_transmitter == NULL) {
    return false;
  }
  RF24* _tranceiver = (RF24*)_transmitter;
  return _tranceiver->write(buf, len);
}

int RF24Receiver::begin(void* radio) {
  if (radio == NULL) {
    return -1;
  }
  _receiver = radio;
  RF24* _tranceiver = (RF24*)_receiver;
  _tranceiver->begin();
  _tranceiver->openReadingPipe(0, address);
  _tranceiver->startListening();
}

int RF24Receiver::check() {
  if (_receiver == NULL) {
    return -1;
  }

  RF24* _tranceiver = (RF24*)_receiver;

  // resets the IRQ pin to HIGH
  bool tx_ds, tx_df, rx_dr;
  _tranceiver->whatHappened(tx_ds, tx_df, rx_dr);
  // returned data should now be reliable
  if (!_tranceiver->available()) {
    return 0;
  }

  uint8_t msg[32] = {0};
  _tranceiver->read(&msg, sizeof(msg));

  uint16_t buttons;
  uint16_t jX, jY;
  uint32_t count;

  bool ok = decodeMessage(msg, "JS", &buttons, &jX, &jY, &count);

#if __RUNNING_LOG_ENABLED__
  Serial.print("decodeMessage -> "), Serial.print(ok);
  Serial.println();
#endif

  if (!ok) {
    return -1;
  }

  JoystickAction message(buttons, jX, jY, count);

  int8_t countNulls = 0, sumFails = 0, sumOk = 0;
  for(int i=0; i<_messageRenderersTotal; i++) {
    int8_t status = invoke(_messageRenderers[i], i+1, &message);
    if (status > 0) {
      sumOk += status;
    } else if (status < 0) {
      sumFails += status;
    } else {
      countNulls++;
    }
  }

  return 0;
}

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

byte RF24Receiver::invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message) {
  if (messageRenderer != NULL) {
    uint8_t code = 1 << index;
    bool ok = messageRenderer->render(message);
#if __RUNNING_LOG_ENABLED__
    Serial.print("#"), Serial.print(message->getFlags()), Serial.print("->"), Serial.print(index), Serial.print(": ");
    if (ok) {
      Serial.println("v");
    } else {
      Serial.println("x");
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
