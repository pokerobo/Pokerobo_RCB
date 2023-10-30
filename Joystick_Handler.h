#ifndef __JOYSTICK_HANDLER_H__
#define __JOYSTICK_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"
#include "Program_Manager.h"

#ifndef __JOYSTICK_READ_BUTTONS_DEBUG__
#define __JOYSTICK_READ_BUTTONS_DEBUG__  0
#endif//__JOYSTICK_READ_BUTTONS_DEBUG__

#ifndef __JOYSTICK_FUNDUINO_SHIELD__
#define __JOYSTICK_FUNDUINO_SHIELD__  1
#endif//__JOYSTICK_FUNDUINO_SHIELD__

#if __JOYSTICK_FUNDUINO_SHIELD__
#define JOYSTICK_MID_X   333
#define JOYSTICK_MID_Y   333
#define JOYSTICK_MAX_X   723
#define JOYSTICK_MAX_Y   723
#else
#define JOYSTICK_MID_X   580
#define JOYSTICK_MID_Y   590
#define JOYSTICK_MAX_X   1024
#define JOYSTICK_MAX_Y   1024
#endif

#ifndef MIN_BOUND_X
#define MIN_BOUND_X      512 - 160
#endif//MIN_BOUND_X

#ifndef MAX_BOUND_X
#define MAX_BOUND_X      512 + 160
#endif//MAX_BOUND_X

#ifndef MIN_BOUND_Y
#define MIN_BOUND_Y      512 - 160
#endif//MIN_BOUND_Y

#ifndef MAX_BOUND_Y
#define MAX_BOUND_Y      512 + 160
#endif//MAX_BOUND_Y

#ifndef PIN_JOYSTICK_X_AXIS
#define PIN_JOYSTICK_X_AXIS   A0
#endif//PIN_JOYSTICK_X_AXIS

#ifndef PIN_JOYSTICK_Y_AXIS
#define PIN_JOYSTICK_Y_AXIS   A1
#endif//PIN_JOYSTICK_Y_AXIS

#ifndef JOYSTICK_CHECKING_CHANGE
#define JOYSTICK_CHECKING_CHANGE 0
#endif

#define PIN_UP_BUTTON     2 // A
#define PIN_RIGHT_BUTTON  3 // B
#define PIN_DOWN_BUTTON   4 // C
#define PIN_LEFT_BUTTON   5 // D
#define PIN_START_BUTTON  6 // F
#define PIN_SELECT_BUTTON 7 // E
#define PIN_ANALOG_BUTTON 8 // JOYSTICK

#define MESSAGE_SENDER_MAX  7

class JoystickHandler {
  static int pinOfButtons[];
#if __STRICT_MODE__
  static void init();
#endif
  public:
    JoystickHandler(MessageSender* messageSender=NULL, MessageRenderer* messageRenderer=NULL);
    int begin();
    int check();
    void detect();
    bool add(MessageSender* messageSender);
  protected:
    uint16_t readButtonStates();
    bool isChanged(int16_t x, int16_t y, uint32_t buttons);
    byte invoke(MessageSender* messageSender, uint8_t index, const void* buf, uint8_t len, MessagePacket* packet=NULL);
  private:
    uint32_t _count = 0;
    int16_t _middleX = JOYSTICK_MID_X;
    int16_t _middleY = JOYSTICK_MID_Y;
    ProgramManager* _programManager = NULL;
    MessageRenderer* _messageRenderer = NULL;
    MessageSender* _messageSenders[MESSAGE_SENDER_MAX] = {};
    uint8_t _messageSendersTotal = 0;
};

#endif//__JOYSTICK_HANDLER_H__
