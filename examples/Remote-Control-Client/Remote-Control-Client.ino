#include <Pokerobo_RCB_worker.h>

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver rf24Tranceiver;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  rf24Tranceiver.add(&displayHandler);
  rf24Tranceiver.begin(RF24_RX, address);
}

void loop() {
  rf24Tranceiver.check();
}
