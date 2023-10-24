#ifndef __MESSAGE_SENDER_H__
#define __MESSAGE_SENDER_H__

#include "Commons.h"

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
};

#endif
