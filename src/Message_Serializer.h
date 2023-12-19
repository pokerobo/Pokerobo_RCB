#ifndef __MESSAGE_SERIALIZER_H__
#define __MESSAGE_SERIALIZER_H__

#include "Commons.h"
#include "Message_Processor.h"

class MessageSerializer {
  public:
    virtual int decode(uint8_t* msg, MessageProcessor *processor);
};

class MovingMessageSerializer: public MessageSerializer {
  public:
    int decode(uint8_t* msg, MessageProcessor *processor);
};

#endif
