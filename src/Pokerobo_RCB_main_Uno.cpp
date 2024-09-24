#include "Configuration.h"

#if defined(ARDUINO_AVR_UNO)
Configuration arduinoUnoR3Conf(
  14,         //JOYSTICK_PIN_X_AXIS(A0)
  15,         //JOYSTICK_PIN_Y_AXIS(A1)
  2,          //PIN_UP_BUTTON
  3,          //PIN_RIGHT_BUTTON
  4,          //PIN_DOWN_BUTTON
  5,          //PIN_LEFT_BUTTON
  6,          //PIN_START_BUTTON
  7,          //PIN_SELECT_BUTTON
  8,          //PIN_ANALOG_BUTTON
  0b1111111,  //JOYSTICK_HIGH_LEVEL_PINS
  0b0000000,  //JOYSTICK_DISABLED_BUTTONS
  9,          //RF24_PRIMARY_PIN_CE
  10,         //RF24_PRIMARY_PIN_CSN
  40,         //RF24_RECEIVER_DISCONTINUITY_MAX
  false,      //RF24_DUPLEX_INTERACTION_ENABLED
  true
);
#endif//ARDUINO_AVR_UNO
