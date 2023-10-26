#ifndef __MESSAGE_SENDER_H__
#define __MESSAGE_SENDER_H__

#include "Commons.h"

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
    uint16_t getButtons();
    uint16_t getX();
    uint16_t getY();
    uint32_t getFlags();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    uint16_t _buttons;
    uint16_t _x;
    uint16_t _y;
    uint32_t _flags;
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
