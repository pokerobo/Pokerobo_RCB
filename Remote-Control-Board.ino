#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"

RF24Tranceiver transmitterInstance;
RF24Tranceiver* transmitter = &transmitterInstance;

JoystickHandler joystickHandlerInstance(transmitter);
JoystickHandler* joystickHandler = &joystickHandlerInstance;

void setup() {
  while (!Serial) delay(100);
  Serial.begin(57600);

  transmitter->begin();
  joystickHandler->begin();
}

void loop() {
  int status = joystickHandler->loop();
  if (status > 1) {
    delay(50);
  } else {
    delay(5);
  }
}
