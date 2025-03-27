#ifndef __POKEROBO_RCB_JOYSTICK_ACTION_H__
#define __POKEROBO_RCB_JOYSTICK_ACTION_H__

#include "Message_Exchange.h"

#define TOTAL_OF_BUTTONS       7

#ifndef BIT_UP_BUTTON
#define BIT_UP_BUTTON          0
#endif//BIT_UP_BUTTON

#ifndef BIT_RIGHT_BUTTON
#define BIT_RIGHT_BUTTON       1
#endif//BIT_RIGHT_BUTTON

#ifndef BIT_DOWN_BUTTON
#define BIT_DOWN_BUTTON        2
#endif//BIT_DOWN_BUTTON

#ifndef BIT_LEFT_BUTTON
#define BIT_LEFT_BUTTON        3
#endif//BIT_LEFT_BUTTON

#ifndef BIT_START_BUTTON
#define BIT_START_BUTTON       4
#endif//BIT_START_BUTTON

#ifndef BIT_SELECT_BUTTON
#define BIT_SELECT_BUTTON      5
#endif//BIT_SELECT_BUTTON

#ifndef BIT_ANALOG_BUTTON
#define BIT_ANALOG_BUTTON      6
#endif//BIT_ANALOG_BUTTON

#define MASK_UP_BUTTON     1U << BIT_UP_BUTTON
#define MASK_RIGHT_BUTTON  1U << BIT_RIGHT_BUTTON
#define MASK_DOWN_BUTTON   1U << BIT_DOWN_BUTTON
#define MASK_LEFT_BUTTON   1U << BIT_LEFT_BUTTON
#define MASK_START_BUTTON  1U << BIT_START_BUTTON
#define MASK_SELECT_BUTTON 1U << BIT_SELECT_BUTTON
#define MASK_ANALOG_BUTTON 1U << BIT_ANALOG_BUTTON

#define MASK_THUMB_ON_LEFT    1U << 12
#define MASK_THUMB_ON_RIGHT   1U << 15
#define MASK_THUMB_ON_UP      1U << 13
#define MASK_THUMB_ON_DOWN    1U << 14

#define POS_UP_BUTTON         2
#define POS_RIGHT_BUTTON      4
#define POS_DOWN_BUTTON       3
#define POS_LEFT_BUTTON       1
#define POS_START_BUTTON      5
#define POS_SELECT_BUTTON     6
#define POS_ANALOG_BUTTON     0

#if __JOYSTICK_FUNDUINO_SHIELD__
#ifndef JOYSTICK_DISABLED_BUTTONS
#define JOYSTICK_DISABLED_BUTTONS 0b0000000
#endif//JOYSTICK_DISABLED_BUTTONS
#else//__JOYSTICK_FUNDUINO_SHIELD__
#ifndef JOYSTICK_DISABLED_BUTTONS
#define JOYSTICK_DISABLED_BUTTONS MASK_UP_BUTTON | \
                                  MASK_RIGHT_BUTTON | \
                                  MASK_DOWN_BUTTON | \
                                  MASK_LEFT_BUTTON | \
                                  MASK_START_BUTTON | \
                                  MASK_SELECT_BUTTON
#endif//JOYSTICK_DISABLED_BUTTONS
#endif//__JOYSTICK_FUNDUINO_SHIELD__

#ifndef JOYSTICK_CHECKING_CHANGE
#define JOYSTICK_CHECKING_CHANGE 0
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

class JoystickAction: public MessageInterface {
  public:
    static const uint8_t messageSize;
    static int16_t getRadiusOfX();
    static int16_t getRadiusOfY();
    JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
    void update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
    JoystickAction(uint16_t x=0, uint16_t y=0, uint16_t pressingFlags=0, uint16_t togglingFlags=0, uint32_t extras=0);
    void update(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras);
    void update(uint16_t x, uint16_t y);
    void setOrigin(uint16_t x, uint16_t y);
    uint16_t getPressingFlags();
    uint16_t getTogglingFlags();
    bool isButtonClicked(uint16_t button);
    bool isButtonPressed(uint16_t button);
    uint16_t getX();
    uint16_t getY();
    uint16_t getOriginX();
    uint16_t getOriginY();
    int16_t getCenterBasedX();
    int16_t getCenterBasedY();
    uint32_t getExtras();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
    MessageInterface* deserialize(uint8_t* buf);
    #if JOYSTICK_CHECKING_CHANGE
    bool isChanged();
    #endif//JOYSTICK_CHECKING_CHANGE
  private:
    uint16_t _pressingFlags = 0;
    uint16_t _togglingFlags = 0;
    uint16_t _x = 0;
    uint16_t _y = 0;
    uint16_t _originX = 0;
    uint16_t _originY = 0;
    uint32_t _extras = 0;
};

// using JoystickControl = JoystickAction;
typedef JoystickAction JoystickControl;

char* buildJoystickActionLogStr(char* log, uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);

#endif
