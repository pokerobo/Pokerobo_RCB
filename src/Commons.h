#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <Arduino.h>

#define __PLATFORM_NANO__                 1
#define __PLATFORM_UNO__                  2
#define __PLATFORM_xNO__                  3
#define __PLATFORM_MEGA2560__             4
#define __PLATFORM_ESP32__                8

#ifndef __PLATFORM_TYPE__
#define __PLATFORM_TYPE__                 __PLATFORM_NANO__
#endif

#define __JOYSTICK_FUNDUINO_SHIELD__      0
#define __JOYSTICK_READ_BUTTONS_DEBUG__   0

/* Attention: MASK_XXX_BUTTON(s) have not defined yet
#define JOYSTICK_DISABLED_BUTTONS MASK_UP_BUTTON | \
                                  MASK_RIGHT_BUTTON | \
                                  MASK_DOWN_BUTTON | \
                                  MASK_LEFT_BUTTON | \
                                  MASK_START_BUTTON
*/
#define JOYSTICK_DISABLED_BUTTONS 0b0101111

#ifndef __RUNNING_LOG_ENABLED__
#define __RUNNING_LOG_ENABLED__           0
#endif

#undef  __DEBUG_LOG_DISPLAY_HANDLER__     1
#undef  __DEBUG_LOG_JOYSTICK_HANDLER__    1
#undef  __DEBUG_LOG_RF24_TRANCEIVER__     1
#undef  __DEBUG_LOG_PROGRAM_SELECTOR__    1

#ifndef __STRICT_MODE__
#define __STRICT_MODE__                   0
#endif

#define __SPACE_SAVING_MODE__             1
#define __OPTIMIZING_DYNAMIC_MEMORY__     1

#ifndef int_abs
#define int_abs(x) ((x)>0?(x):-(x))
#endif

#ifndef int_min
#define int_min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef int_max
#define int_max(a,b) ((a)>(b)?(a):(b))
#endif

#if __JOYSTICK_FUNDUINO_SHIELD__
#define JOYSTICK_MID_X   333
#define JOYSTICK_MID_Y   333
#define JOYSTICK_MAX_X   723
#define JOYSTICK_MAX_Y   723
#endif

#if __PLATFORM_TYPE__ != __PLATFORM_ESP32__
#define WIRE_HAS_TIMEOUT                  1
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_ESP32__
#define PIN_CE  4
#define PIN_CSN 5
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_ESP32__
#define JOYSTICK_PIN_X_AXIS   36
#define JOYSTICK_PIN_Y_AXIS   39
#define PIN_UP_BUTTON     27 // A
#define PIN_RIGHT_BUTTON  26 // B
#define PIN_DOWN_BUTTON   25 // C
#define PIN_LEFT_BUTTON   33 // D
#define PIN_START_BUTTON  34 // F
#define PIN_SELECT_BUTTON 35 // E
#define PIN_ANALOG_BUTTON 32 // JOYSTICK
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_ESP32__
#define JOYSTICK_HIGH_LEVEL_PINS  0b1000000
#define JOYSTICK_DISABLED_BUTTONS 0b0111111
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_ESP32__
#define JOYSTICK_MID_X   2810
#define JOYSTICK_MID_Y   2810
#define JOYSTICK_MAX_X   4095
#define JOYSTICK_MAX_Y   4095
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_MEGA2560__
#define PIN_CE  48
#define PIN_CSN 49
#endif//__PLATFORM_TYPE__

#endif
