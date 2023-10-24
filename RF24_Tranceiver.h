#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include "Commons.h"
#include "Message_Sender.h"

class RF24Tranceiver: public MessageSender {
  public:
    int begin();
    bool write(const void* buf, uint8_t len);
};

#endif
