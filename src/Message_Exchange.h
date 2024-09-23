#ifndef __POKEROBO_RCB_MESSAGE_EXCHANGE_H__
#define __POKEROBO_RCB_MESSAGE_EXCHANGE_H__

#include "Commons.h"

#ifndef MESSAGE_SIGNATURE
#define MESSAGE_SIGNATURE         "JS"
#endif//MESSAGE_SIGNATURE

class MessageInterface {
  public:
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
    virtual void* deserialize(uint8_t* buf);
};

class MasterContext: public MessageInterface {
  public:
    MasterContext();
    MasterContext(uint8_t applicationId, bool programCheckBit);
    MasterContext(uint8_t* buf);
    uint8_t getApplicationId();
    bool getContextCheckBit();
    bool getProgramCheckBit();

    static const uint8_t messageSize;
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
    MessageInterface* deserialize(uint8_t* buf);
  private:
    int _errorCode = 0;
    bool _debugEnabled = true;
    uint8_t _applicationId = 0;
    bool _contextCheckBit = 0;
    bool _programCheckBit = 0;
};

class MessagePacket: public MessageInterface {
  public:
    MessagePacket(MessageInterface* context, MessageInterface* control, MessageInterface* command);
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    uint8_t* _signature = (uint8_t*)MESSAGE_SIGNATURE;
    MessageInterface* _context = NULL;
    MessageInterface* _control = NULL;
    MessageInterface* _command = NULL;
};

class MessageSender {
  public:
    virtual bool write(const void* buf, uint8_t len);
    virtual bool write(MessagePacket* packet);
};

uint8_t* encodeInteger(uint8_t* store, uint16_t value);
uint8_t* encodeInteger(uint8_t* store, uint32_t value);
uint32_t decodeInteger(uint8_t* arr, int length);

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
uint8_t* encodeMessage(uint8_t* msg, char* cmd,
    uint16_t x, uint16_t y,
    uint16_t pressingFlags,
    uint16_t togglingFlags,
    uint32_t extras);

bool decodeMessage(uint8_t* buf, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras);
bool decodeMessage(uint8_t* msg, char* cmd,
    uint16_t* x, uint16_t* y,
    uint16_t* pressingFlags,
    uint16_t* togglingFlags,
    uint32_t* extras);

void convertByteToHexString(uint8_t addr, char* s);

#endif
