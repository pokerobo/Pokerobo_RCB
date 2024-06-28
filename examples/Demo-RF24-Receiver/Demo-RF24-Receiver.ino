#include <Pokerobo_RCB.h>

const uint8_t address = 1;

DisplayHandler displayHandler;
JoystickMessageSerializer messageSerializer;
RF24Tranceiver rf24Receiver;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  rf24Receiver.set(&displayHandler);
  rf24Receiver.set(&messageSerializer);
  rf24Receiver.begin(RF24_RX, RF24_BASE_ADDRESS + address);
}

void loop() {
  rf24Receiver.check();
}
