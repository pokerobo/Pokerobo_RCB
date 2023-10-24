#ifndef __MESSAGE_SENDER_H__
#define __MESSAGE_SENDER_H__

#include "Commons.h"

#define MESSAGE_RENDERERS_LIMIT   7

class MessagePacket {
  public:
    MessagePacket(uint16_t buttons, uint16_t x, uint16_t y, uint32_t flags);
    uint16_t getButtons();
    uint16_t getX();
    uint16_t getY();
    uint32_t getFlags();
  private:
    uint16_t _buttons;
    uint16_t _x;
    uint16_t _y;
    uint32_t _flags;
};

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
};

class MessageRenderer {
  public:
    virtual bool write(const void* buf, uint8_t len);
};

class ConsoleMessageRenderer: public MessageRenderer {

};

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t flags);

bool decodeMessage(uint8_t* buf, char* cmd, uint16_t* pressed, uint16_t* x, uint16_t* y, uint32_t* flags);

#endif
