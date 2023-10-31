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

#define MESSAGE_RENDERERS_LIMIT   7

class MessagePacket {
  public:
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
};

class JoystickAction: public MessagePacket {
  static const uint8_t messageSize;
  public:
    JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t flags);
    void setOrigin(uint16_t x, uint16_t y);
    uint16_t getButtons();
    uint16_t getX();
    uint16_t getY();
    uint16_t getOriginX();
    uint16_t getOriginY();
    uint32_t getFlags();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    uint16_t _buttons = 0;
    uint16_t _x = 0;
    uint16_t _y = 0;
    uint16_t _originX = 0;
    uint16_t _originY = 0;
    uint32_t _flags = 0;
};

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
    virtual bool write(MessagePacket* packet);
};

class MessageRenderer {
  public:
    virtual bool render(JoystickAction* message);
};

class ConsoleMessageRenderer: public MessageRenderer {
  public:
    bool render(JoystickAction* message);
};

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t flags);

bool decodeMessage(uint8_t* buf, char* cmd, uint16_t* pressed, uint16_t* x, uint16_t* y, uint32_t* flags);

#endif
