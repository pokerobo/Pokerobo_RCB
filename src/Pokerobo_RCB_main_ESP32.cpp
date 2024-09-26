#include <Arduino.h>
#include "Configuration.h"

#if defined(ARDUINO_ARCH_ESP32)

Configuration doitESP32Conf(
  36,         //JOYSTICK_PIN_X_AXIS
  39,         //JOYSTICK_PIN_Y_AXIS
  27,         //PIN_UP_BUTTON
  26,         //PIN_RIGHT_BUTTON
  25,         //PIN_DOWN_BUTTON
  33,         //PIN_LEFT_BUTTON
  34,         //PIN_START_BUTTON
  35,         //PIN_SELECT_BUTTON
  32,         //PIN_ANALOG_BUTTON
  0b1000000,  //JOYSTICK_HIGH_LEVEL_PINS
  0b0111111,  //JOYSTICK_DISABLED_BUTTONS
  4,          //RF24_PRIMARY_PIN_CE
  5,          //RF24_PRIMARY_PIN_CSN
  5000,       //RF24_RECEIVER_DISCONTINUITY_MAX
  false,      //RF24_TWO_WAY_ENABLED
  true
);

#endif//ARDUINO_ARCH_ESP32
