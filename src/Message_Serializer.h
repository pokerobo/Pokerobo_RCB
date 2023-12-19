#ifndef __MESSAGE_SERIALIZER_H__
#define __MESSAGE_SERIALIZER_H__

#include "Commons.h"
#include "Message_Processor.h"

class MessageSerializer {
  public:
    virtual uint8_t getSize();
    virtual int decode(uint8_t* msg, MessageProcessor *processor);
};

#endif
