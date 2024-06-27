#include <Pokerobo_RCB.h>

DeviceDisplayHandler displayHandler;
JoystickHandler joystickHandler;

void setup() {
  Serial.begin(57600);
  joystickHandler.begin();
  displayHandler.begin();

  displayHandler.render(NULL);
}

void loop() {}
