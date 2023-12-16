#ifndef __MESSAGE_EXCHANGE_H__
#define __MESSAGE_EXCHANGE_H__

#include "Commons.h"

#ifndef MESSAGE_SIGNATURE
#define MESSAGE_SIGNATURE         "JS"
#endif//MESSAGE_SIGNATURE

typedef struct _TransmissionCounter {
  uint32_t baselineNumber = 0;
  uint32_t ordinalNumber = 0;
  uint32_t continualLossCount = 0;
  uint32_t packetLossTotal = 0;
} TransmissionCounter;

class MessageInterface {
  public:
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
};

class MessagePacket: public MessageInterface {
  public:
    MessagePacket(MessageInterface* action, MessageInterface* command=NULL);
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    uint8_t* _signature = (uint8_t*)MESSAGE_SIGNATURE;
    MessageInterface* _action = NULL;
    MessageInterface* _command = NULL;
};

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
    virtual bool write(MessagePacket* packet);
};

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t extras);
bool decodeMessage(uint8_t* buf, char* cmd, uint16_t* pressed, uint16_t* x, uint16_t* y, uint32_t* extras);

#endif
