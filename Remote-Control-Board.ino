#include "Display_Handler.h"
#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"

DisplayHandler displayHandlerInstance;
DisplayHandler* displayHandler = &displayHandlerInstance;

RF24Tranceiver tranceiverInstance;
RF24Tranceiver* tranceiver = &tranceiverInstance;

JoystickHandler joystickHandlerInstance(&tranceiverInstance, displayHandler);
JoystickHandler* joystickHandler = &joystickHandlerInstance;

ConsoleMessageRenderer consoleMessageRenderer;

void setup() {
  while (!Serial) delay(100);
  Serial.begin(57600);

  displayHandler->begin();

#if __RF24_TRANCEIVER_MODE__
  tranceiver->begin();
  joystickHandler->begin();
#else
  tranceiver->begin(RX);
  tranceiver->add(&consoleMessageRenderer);
#endif
}

void loop() {
#if __RF24_TRANCEIVER_MODE__
  int status = joystickHandler->check();
  if (status > 1) {
    delay(50);
  } else {
    delay(5);
  }
#else
  tranceiver->check();
  delay(50);
#endif
}
