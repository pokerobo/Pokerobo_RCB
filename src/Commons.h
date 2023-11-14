#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <Arduino.h>

#define __PLATFORM_NANO__                 1
#define __PLATFORM_UNO__                  2
#define __PLATFORM_MEGA2560__             4
#define __PLATFORM_ESP32__                8

#ifndef __PLATFORM_TYPE__
#define __PLATFORM_TYPE__                 __PLATFORM_NANO__
#endif

#if __PLATFORM_TYPE__ != __PLATFORM_ESP32__
#define WIRE_HAS_TIMEOUT                  1
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

#undef __DEBUG_LOG_DISPLAY_HANDLER__      0
#undef __DEBUG_LOG_JOYSTICK_HANDLER__     0
#undef __DEBUG_LOG_RF24_TRANCEIVER__      0

#ifndef __STRICT_MODE__
#define __STRICT_MODE__                   0
#endif

#ifndef __SPACE_SAVING_MODE__
#define __SPACE_SAVING_MODE__             1
#endif

#ifndef int_abs
#define int_abs(x) ((x)>0?(x):-(x))
#endif

#ifndef int_min
#define int_min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef int_max
#define int_max(a,b) ((a)>(b)?(a):(b))
#endif

#endif
