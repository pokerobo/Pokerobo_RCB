#include <Pokerobo_RCB_worker.h>

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver tranceiver;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  tranceiver.add(&displayHandler);
  tranceiver.begin(RF24_RX, address);
}

void loop() {
  tranceiver.check();
}
