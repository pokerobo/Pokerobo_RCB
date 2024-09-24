#ifndef __POKEROBO_RCB_COMMONS_H__
#define __POKEROBO_RCB_COMMONS_H__

#include <Arduino.h>

#define __PLATFORM_NANO__                 0b0001
#define __PLATFORM_UNO__                  0b0010
#define __PLATFORM_MEGA2560__             0b0100
#define __PLATFORM_xNO__                  __PLATFORM_NANO__ | __PLATFORM_UNO__
#define __PLATFORM_ARDUINO__              __PLATFORM_xNO__ | __PLATFORM_MEGA2560__

#define __PLATFORM_DOIT_ESP32_V1__        0b100000
#define __PLATFORM_WEMOS_D1_R32__         0b010000
#define __PLATFORM_ESP32__                __PLATFORM_DOIT_ESP32_V1__ | __PLATFORM_WEMOS_D1_R32__

#ifndef __PLATFORM_TYPE__
#define __PLATFORM_TYPE__                 __PLATFORM_UNO__
#endif

#define __JOYSTICK_FUNDUINO_SHIELD__      1
#define __JOYSTICK_READ_BUTTONS_DEBUG__   0

/* Attention: MASK_XXX_BUTTON(s) have not defined yet
#define JOYSTICK_DISABLED_BUTTONS MASK_UP_BUTTON | \
                                  MASK_RIGHT_BUTTON | \
                                  MASK_DOWN_BUTTON | \
                                  MASK_LEFT_BUTTON | \
                                  MASK_START_BUTTON
#define JOYSTICK_DISABLED_BUTTONS 0b0101111
*/

#ifndef __RUNNING_LOG_ENABLED__
#define __RUNNING_LOG_ENABLED__           0
#endif

#undef  __DEBUG_LOG_DISPLAY_HANDLER__     1
#undef  __DEBUG_LOG_JOYSTICK_HANDLER__    1
#undef  __DEBUG_LOG_RF24_TRANCEIVER__     1
#undef  __DEBUG_LOG_PROGRAM_SELECTOR__    1
#define __DEBUG_LOG_COMMAND_RESOLVER__    0

#ifndef __STRICT_MODE__
#define __STRICT_MODE__                   0
#endif

#define __SPACE_SAVING_MODE__             1
#define __OPTIMIZING_DYNAMIC_MEMORY__     0

#ifndef int_abs
#define int_abs(x) ((x)>0?(x):-(x))
#endif

#ifndef int_min
#define int_min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef int_max
#define int_max(a,b) ((a)>(b)?(a):(b))
#endif

//-------------------------------------------------------------------------------------------------

#define POKEROBO_DYNAMIC_CONFIGURATION    1

#define CONTROL_PACKET_V1                 1
#define CONTROL_PACKET_V2                 2

#ifndef __JOYSTICK_MESSAGE_STRUCTURE__
#define __JOYSTICK_MESSAGE_STRUCTURE__    CONTROL_PACKET_V1
#endif//__JOYSTICK_MESSAGE_STRUCTURE__

#if __PLATFORM_TYPE__ & __PLATFORM_ARDUINO__
#define WIRE_HAS_TIMEOUT_ENABLED          1
#endif

//-------------------------------------------------------------------------------------------------

#if __PLATFORM_TYPE__ & __PLATFORM_NANO__
#define __JOYSTICK_FUNDUINO_SHIELD__    0
#endif

#if __PLATFORM_TYPE__ & __PLATFORM_NANO__
#define RF24_RECEIVER_DISCONTINUITY_MAX 40
#endif

#if __PLATFORM_TYPE__ & __PLATFORM_NANO__
#define JOYSTICK_MID_X   580
#define JOYSTICK_MID_Y   590
#define JOYSTICK_MAX_X   900
#define JOYSTICK_MAX_Y   900
#endif

//-------------------------------------------------------------------------------------------------

#if __PLATFORM_TYPE__ == __PLATFORM_DOIT_ESP32_V1__
#define __JOYSTICK_FUNDUINO_SHIELD__    0
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_DOIT_ESP32_V1__
#define RF24_PRIMARY_PIN_CE             4
#define RF24_PRIMARY_PIN_CSN            5
#define RF24_RECEIVER_DISCONTINUITY_MAX 5000
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_DOIT_ESP32_V1__
#define JOYSTICK_PIN_X_AXIS   36
#define JOYSTICK_PIN_Y_AXIS   39
#define PIN_UP_BUTTON         27 // A
#define PIN_RIGHT_BUTTON      26 // B
#define PIN_DOWN_BUTTON       25 // C
#define PIN_LEFT_BUTTON       33 // D
#define PIN_START_BUTTON      34 // F
#define PIN_SELECT_BUTTON     35 // E
#define PIN_ANALOG_BUTTON     32 // JOYSTICK
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_DOIT_ESP32_V1__
#define JOYSTICK_HIGH_LEVEL_PINS  0b1000000
#define JOYSTICK_DISABLED_BUTTONS 0b0111111
#endif

#if __PLATFORM_TYPE__ == __PLATFORM_DOIT_ESP32_V1__
// with Vin ~ 3V3
#define JOYSTICK_MID_X   1920   // 2810 (5V)
#define JOYSTICK_MID_Y   1920   // 2810 (5V)
#define JOYSTICK_MAX_X   4095
#define JOYSTICK_MAX_Y   4095
#endif

//-------------------------------------------------------------------------------------------------

#if __PLATFORM_TYPE__ == __PLATFORM_WEMOS_D1_R32__
#define RF24_PRIMARY_PIN_CE             13
#define RF24_PRIMARY_PIN_CSN            05
#define RF24_RECEIVER_DISCONTINUITY_MAX 5000
#endif//__PLATFORM_TYPE__

#if __PLATFORM_TYPE__ == __PLATFORM_WEMOS_D1_R32__
#define JOYSTICK_PIN_X_AXIS   02
#define JOYSTICK_PIN_Y_AXIS   04
#define PIN_UP_BUTTON         26 // A
#define PIN_RIGHT_BUTTON      25 // B
#define PIN_DOWN_BUTTON       17 // C
#define PIN_LEFT_BUTTON       16 // D
#define PIN_START_BUTTON      27 // F
#define PIN_SELECT_BUTTON     14 // E
#define PIN_ANALOG_BUTTON     12 // JOYSTICK
#endif

//-------------------------------------------------------------------------------------------------

#if __PLATFORM_TYPE__ == __PLATFORM_MEGA2560__
#define RF24_PRIMARY_PIN_CE  48
#define RF24_PRIMARY_PIN_CSN 49
#endif//__PLATFORM_TYPE__

//-------------------------------------------------------------------------------------------------

#if __JOYSTICK_FUNDUINO_SHIELD__
#define JOYSTICK_MID_X   300
#define JOYSTICK_MID_Y   300
#define JOYSTICK_MAX_X   600
#define JOYSTICK_MAX_Y   600
#endif

#ifndef RF24_DUPLEX_INTERACTION_ENABLED
#define RF24_DUPLEX_INTERACTION_ENABLED                    0
#endif//RF24_DUPLEX_INTERACTION_ENABLED

#endif
