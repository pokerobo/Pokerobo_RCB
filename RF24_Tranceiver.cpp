#include "RF24_Tranceiver.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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
  Serial.print("#"), Serial.print(count), Serial.print(" - ");
  Serial.print("Buttons"), Serial.print(": "), Serial.print(buttons);
  Serial.print("; "), Serial.print("X"), Serial.print(": "), Serial.print(jX);
  Serial.print("; "), Serial.print("Y"), Serial.print(": "), Serial.print(jY);
  Serial.println();
#endif

  if (!ok) {
    return -1;
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
