#ifndef __JOYSTICK_HANDLER_H__
#define __JOYSTICK_HANDLER_H__

#include "Commons.h"
#include "Joystick_Action.h"
#include "Message_Exchange.h"
#include "Message_Renderer.h"
#include "Moving_Resolver.h"

#ifndef __JOYSTICK_READ_BUTTONS_DEBUG__
#define __JOYSTICK_READ_BUTTONS_DEBUG__   0
#endif//__JOYSTICK_READ_BUTTONS_DEBUG__

#ifndef __JOYSTICK_FUNDUINO_SHIELD__
#define __JOYSTICK_FUNDUINO_SHIELD__      1
#endif//__JOYSTICK_FUNDUINO_SHIELD__

#ifndef JOYSTICK_MID_X
#define JOYSTICK_MID_X   580
#endif//JOYSTICK_MID_X

#ifndef JOYSTICK_MID_Y
#define JOYSTICK_MID_Y   590
#endif//JOYSTICK_MID_Y

#ifndef JOYSTICK_MAX_X
#define JOYSTICK_MAX_X   900
#endif//JOYSTICK_MAX_X

#ifndef JOYSTICK_MAX_Y
#define JOYSTICK_MAX_Y   900
#endif//JOYSTICK_MAX_Y

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

#ifndef JOYSTICK_PIN_X_AXIS
#define JOYSTICK_PIN_X_AXIS   A0
#endif//JOYSTICK_PIN_X_AXIS

#ifndef JOYSTICK_PIN_Y_AXIS
#define JOYSTICK_PIN_Y_AXIS   A1
#endif//JOYSTICK_PIN_Y_AXIS

#ifndef JOYSTICK_CHECKING_CHANGE
#define JOYSTICK_CHECKING_CHANGE 0
#endif

#ifndef PIN_UP_BUTTON
#define PIN_UP_BUTTON     2 // A
#endif
#ifndef PIN_RIGHT_BUTTON
#define PIN_RIGHT_BUTTON  3 // B
#endif
#ifndef PIN_DOWN_BUTTON
#define PIN_DOWN_BUTTON   4 // C
#endif
#ifndef PIN_LEFT_BUTTON
#define PIN_LEFT_BUTTON   5 // D
#endif
#ifndef PIN_START_BUTTON
#define PIN_START_BUTTON  6 // F
#endif
#ifndef PIN_SELECT_BUTTON
#define PIN_SELECT_BUTTON 7 // E
#endif
#ifndef PIN_ANALOG_BUTTON
#define PIN_ANALOG_BUTTON 8 // JOYSTICK
#endif

#define MESSAGE_EXCHANGE_MAX  7
#define MULTIPLE_SENDERS_SUPPORTED false

class JoystickHandler {
  static const int pinOfButtons[];
  #if __STRICT_MODE__
  static void verify();
  #endif
  public:
    int begin();
    JoystickAction input();
    JoystickAction* input(JoystickAction* packet);
    uint8_t checkArrowKeysToggle(uint16_t x, uint16_t y);
    #if __JOYSTICK_HANDLER_CHECK_ENABLED__
    int check(JoystickAction* action=NULL);
    #if MULTIPLE_SENDERS_SUPPORTED
    bool add(MessageSender* messageSender);
    #endif//MULTIPLE_SENDERS_SUPPORTED
    void set(MessageSender* messageSender);
    void set(MessageRenderer* messageRenderer);
    void set(MovingResolver* movingResolver);
    #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
  protected:
    void detect();
    uint16_t readButtonStates();
    uint16_t checkButtonClickingFlags(uint16_t pressed);
    #if __JOYSTICK_HANDLER_CHECK_ENABLED__
    #if JOYSTICK_CHECKING_CHANGE
    bool isChanged(JoystickAction* msg);
    #endif//JOYSTICK_CHECKING_CHANGE
    #if MULTIPLE_SENDERS_SUPPORTED
    byte invoke(MessageSender* messageSender, uint8_t index, const void* buf, uint8_t len, MessagePacket* packet=NULL);
    #endif//MULTIPLE_SENDERS_SUPPORTED
    #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
  private:
    uint32_t _ordinalNumber = 0;
    uint16_t _clickingTrail = 0;
    int16_t _middleX = JOYSTICK_MID_X;
    int16_t _middleY = JOYSTICK_MID_Y;
    int16_t _maxX = JOYSTICK_MAX_X;
    int16_t _maxY = JOYSTICK_MAX_Y;
    bool _arrowKeysToggleTrapped = true;
    uint8_t _arrowKeysToggleTrail = 0;
    #if __JOYSTICK_HANDLER_CHECK_ENABLED__
    TransmissionCounter _counter;
    MessageRenderer* _messageRenderer = NULL;
    #if MULTIPLE_SENDERS_SUPPORTED
    MessageSender* _messageSenders[MESSAGE_EXCHANGE_MAX] = {};
    uint8_t _messageSendersTotal = 0;
    #endif//MULTIPLE_SENDERS_SUPPORTED
    MessageSender* _messageSender = NULL;
    MovingResolver* _movingResolver = NULL;
    #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
};

#endif//__JOYSTICK_HANDLER_H__
