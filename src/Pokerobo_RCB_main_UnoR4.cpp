#include <Arduino.h>
#include "Configuration.h"

#if defined(ARDUINO_UNOR4_WIFI)

Configuration arduinoUnoR4Conf(
  A0,         //JOYSTICK_PIN_X_AXIS(A0)
  A1,         //JOYSTICK_PIN_Y_AXIS(A1)
  D2,         //PIN_UP_BUTTON
  D3,         //PIN_RIGHT_BUTTON
  D4,         //PIN_DOWN_BUTTON
  D5,         //PIN_LEFT_BUTTON
  D6,         //PIN_START_BUTTON
  D7,         //PIN_SELECT_BUTTON
  D8,         //PIN_ANALOG_BUTTON
  0b1111111,  //JOYSTICK_HIGH_LEVEL_PINS
  0b0000000,  //JOYSTICK_DISABLED_BUTTONS
  D9,         //RF24_PRIMARY_PIN_CE
  D10,        //RF24_PRIMARY_PIN_CSN
  5000,       //RF24_RECEIVER_DISCONTINUITY_MAX
  false,      //RF24_DUPLEX_INTERACTION_ENABLED
  true
);

#endif//ARDUINO_UNOR4_WIFI
