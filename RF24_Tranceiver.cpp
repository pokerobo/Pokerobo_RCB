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
RF24* transmitter = &radio;

int RF24Tranceiver::begin() {
  transmitter->begin();
  transmitter->openWritingPipe(address);
  transmitter->stopListening();
}

bool RF24Tranceiver::write(const void* buf, uint8_t len) {
  return transmitter->write(buf, len);
}
