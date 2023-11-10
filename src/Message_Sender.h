#ifndef __MESSAGE_SENDER_H__
#define __MESSAGE_SENDER_H__

#include "Commons.h"

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

#ifndef MESSAGE_SIGNATURE
#define MESSAGE_SIGNATURE         "JS"
#endif//MESSAGE_SIGNATURE

typedef enum { TX_MSG = 0, RX_MSG } message_source_t;

typedef struct _TransmissionCounter {
  uint32_t baselineNumber = 0;
  uint32_t ordinalNumber = 0;
  uint32_t packetLossTotal = 0;
} TransmissionCounter;

class MessagePacket {
  public:
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
};

class JoystickAction: public MessagePacket {
  public:
    static const uint8_t messageSize;
    JoystickAction(uint16_t buttons=0, uint16_t x=0, uint16_t y=0, uint32_t extras=0);
    void update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
    void setOrigin(uint16_t x, uint16_t y);
    void setClickingFlags(uint16_t clickingFlags);
    void setSource(message_source_t source);
    message_source_t getSource();
    uint16_t getPressingFlags();
    uint16_t getClickingFlags();
    uint16_t getX();
    uint16_t getY();
    uint16_t getOriginX();
    uint16_t getOriginY();
    uint32_t getExtras();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    uint16_t _pressingFlags = 0;
    uint16_t _clickingTrail = 0;
    uint16_t _x = 0;
    uint16_t _y = 0;
    uint16_t _originX = 0;
    uint16_t _originY = 0;
    uint32_t _extras = 0;
    message_source_t _source = TX_MSG;
};

class SpeedPacket {
  public:
    SpeedPacket(int leftSpeed=0, byte leftDirection=0, int rightSpeed=0, byte rightDirection=0);
    void update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection);
    int getLeftSpeed();
    byte getLeftDirection();
    int getRightSpeed();
    byte getRightDirection();
  private:
    int _LeftSpeed;
    byte _LeftDirection;
    int _RightSpeed;
    byte _RightDirection;
};

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
    virtual bool write(MessagePacket* packet);
};

class MessageRenderer {
  public:
    virtual void clear();
    virtual void splash(char* title, byte align = 0);
    virtual void render(JoystickAction* message, SpeedPacket* speedPacket=NULL, TransmissionCounter* counter=NULL);
};

class ConsoleMessageRenderer: public MessageRenderer {
  public:
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, SpeedPacket* speedPacket=NULL, TransmissionCounter* counter=NULL);
};

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t extras);

bool decodeMessage(uint8_t* buf, char* cmd, uint16_t* pressed, uint16_t* x, uint16_t* y, uint32_t* extras);

#endif
