#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"

RF24Tranceiver tranceiverInstance;
RF24Tranceiver* tranceiver = &tranceiverInstance;

JoystickHandler joystickHandlerInstance(&tranceiverInstance);
JoystickHandler* joystickHandler = &joystickHandlerInstance;

void setup() {
  while (!Serial) delay(100);
  Serial.begin(57600);

  tranceiver->begin();
  joystickHandler->begin();
}

void loop() {
  int status = joystickHandler->check();
  if (status > 1) {
    delay(50);
  } else {
    delay(5);
  }
}
