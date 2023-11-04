#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <Arduino.h>

#define __AVR_NANO__                      1
#define __AVR_UNO__                       2
#define __AVR_MEGA_2560__                 4

#ifndef __AVR_MODEL__
#define __AVR_MODEL__                     __AVR_UNO__
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
#define JOYSTICK_DISABLED_BUTTONS 0b0011111

#ifndef __RUNNING_LOG_ENABLED__
#define __RUNNING_LOG_ENABLED__           1
#endif

#ifndef __STRICT_MODE__
#define __STRICT_MODE__                   0
#endif

#endif
