#ifndef __POKEROBO_RCB_MESSAGE_SERIALIZER_H__
#define __POKEROBO_RCB_MESSAGE_SERIALIZER_H__

#include "Commons.h"
#include "Message_Processor.h"

class MessageSerializer {
  public:
    virtual uint8_t getSize();
    virtual int decode(uint8_t* msg, MessageProcessor *processor);
};

class JoystickMessageSerializer: public MessageSerializer {
  public:
    uint8_t getSize();
    int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};

#endif
