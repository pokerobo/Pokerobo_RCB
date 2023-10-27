#ifndef __COMMONS_H__
#define __COMMONS_H__

#define __RUNNING_LOG_ENABLED__           1
#define __JOYSTICK_FUNDUINO_SHIELD__      0
#define __JOYSTICK_READ_BUTTONS_DEBUG__   0

#include <Arduino.h>

#ifndef __RUNNING_LOG_ENABLED__
#define __RUNNING_LOG_ENABLED__           1
#endif

#ifndef __STRICT_MODE__
#define __STRICT_MODE__                   0
#endif

#ifndef __RF24_TRANCEIVER_MODE__
#define __RF24_TRANCEIVER_MODE__          0
#endif

#endif
