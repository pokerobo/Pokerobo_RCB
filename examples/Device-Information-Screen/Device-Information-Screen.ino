#include <Pokerobo_RCB.h>

JoystickHandler joystickHandler;
DeviceDisplayHandler displayHandler;

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  displayHandler.render(NULL);
}

void loop() {}
