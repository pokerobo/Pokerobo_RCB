#include <Pokerobo_RCB_master.h>

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver tranceiver;
JoystickHandler joystickHandler(&tranceiver, &displayHandler);
SpeedResolver speedResolver;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();
  tranceiver.begin(TX, address);
  joystickHandler.begin();
  joystickHandler.set(&speedResolver);
}

void loop() {
  int status = joystickHandler.check();
  delay((status > 1) ? 10 : 5);
}
