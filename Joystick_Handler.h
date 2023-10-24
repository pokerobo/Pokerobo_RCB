#ifndef __JOYSTICK_HANDLER_H__
#define __JOYSTICK_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"

#ifndef __FUNDUINO_JOYSTICK_SHIELD__
#define __FUNDUINO_JOYSTICK_SHIELD__  1
#endif//__FUNDUINO_JOYSTICK_SHIELD__

#if __FUNDUINO_JOYSTICK_SHIELD__
#define JOYSTICK_MAX_X   723
#define JOYSTICK_MAX_Y   723
#else
#define JOYSTICK_MAX_X   950
#define JOYSTICK_MAX_Y   950
#endif

#define MIN_BOUND_X      512 - 64
#define MAX_BOUND_X      512 + 64
#define MIN_BOUND_Y      512 - 64
#define MAX_BOUND_Y      512 + 64

#define BIT_UP_BUTTON     1 << 0
#define BIT_RIGHT_BUTTON  1 << 1
#define BIT_DOWN_BUTTON   1 << 2
#define BIT_LEFT_BUTTON   1 << 3
#define BIT_START_BUTTON  1 << 4
#define BIT_SELECT_BUTTON 1 << 5
#define BIT_ANALOG_BUTTON 1 << 6

#define PIN_UP_BUTTON     2 // A
#define PIN_RIGHT_BUTTON  3 // B
#define PIN_DOWN_BUTTON   4 // C
#define PIN_LEFT_BUTTON   5 // D
#define PIN_START_BUTTON  6 // F
#define PIN_SELECT_BUTTON 7 // E
#define PIN_ANALOG_BUTTON 8 // JOYSTICK

uint16_t readButtonStates();

class JoystickHandler {
  public:
    JoystickHandler(MessageSender* sender);
    int begin();
    int loop();
  private:
    MessageSender* _messageSender;
};

#endif//__JOYSTICK_HANDLER_H__
