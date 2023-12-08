#include <Pokerobo_RCB_master.h>

JoystickHandler joystickHandler;

void setup() {
  Serial.begin(57600);
  joystickHandler.begin();
}

void loop() {
  joystickHandler.check(NULL);
}
