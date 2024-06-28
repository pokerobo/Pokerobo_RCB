#include <Pokerobo_RCB.h>

const uint64_t address = 0x18580901LL;

JoystickAction joystickAction;
JoystickHandler joystickHandler;
DisplayHandler displayHandler;
RF24Tranceiver rf24Transmitter;

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  rf24Transmitter.begin(RF24_TX, address);  
}

void loop() {
  joystickHandler.input(&joystickAction);
  displayHandler.render(&joystickAction);
  MessagePacket packet(NULL, &joystickAction, NULL);
  rf24Transmitter.write(&packet);
  delay(50);
}
