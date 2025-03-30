#ifndef __POKEROBO_RCB_H__
#define __POKEROBO_RCB_H__

#include <Arduino.h>

#if defined(ARDUINO_AVR_UNO)
#include "Pokerobo_RCB_main_Uno.h"
#endif

#if defined(ARDUINO_UNOR4_WIFI)
#include "Pokerobo_RCB_main_UnoR4.h"
#endif

#if defined(ARDUINO_AVR_NANO)
#include "Pokerobo_RCB_main_Nano.h"
#endif

#if defined(ARDUINO_ARCH_ESP32)
#include "Pokerobo_RCB_main_ESP32.h"
#endif

#endif
