#ifndef __POKEROBO_RCB_MESSAGE_MONITOR_H__
#define __POKEROBO_RCB_MESSAGE_MONITOR_H__

#include "Commons.h"

#ifndef RF24_BASE_ADDRESS
#define RF24_BASE_ADDRESS             0x18580900LL
#endif//RF24_BASE_ADDRESS

#ifndef RF24_DEFAULT_ADDRESS
#define RF24_DEFAULT_ADDRESS          0x123456789ABCDEF0LL
#endif//RF24_DEFAULT_ADDRESS

#define DEFAULT_OFFSET_ADDRESS        0

typedef enum { RF24_TX = 0, RF24_RX } tranceiver_t;

struct TransmissionProfile {
  public:
    TransmissionProfile(tranceiver_t mode, uint8_t offsetAddress);
    tranceiver_t getMode();
    void setOffsetAddress(uint8_t offsetAddress);
    uint8_t getOffsetAddress();
    uint64_t getRF24Address();
    uint64_t getBaseAddress();
  private:
    tranceiver_t _mode = RF24_TX;
    uint8_t _offsetAddress = DEFAULT_OFFSET_ADDRESS;
};

class TransmissionContext {
  public:
    void setTransmissionProfile(TransmissionProfile* profile);
    TransmissionProfile* getTransmissionProfile();
  private:
    TransmissionProfile* _profile = NULL;
};

typedef struct _TransmissionCounter {
  private:
    uint32_t baselineNumber = 0;
    uint32_t ordinalNumber = 0;
    uint32_t continualLossCount = 0;
    uint32_t packetLossTotal = 0;

  public:
    uint32_t getBaselineNumber();
    uint32_t getOrdinalNumber();
    uint32_t getContinualLossCount();
    uint32_t getPacketLossTotal();
    void adjust();
    void reset();
    void beginTransmission();
    void confirmTransmissionSuccess();
    void confirmTransmissionFailure();
    void update(uint32_t count);
} TransmissionCounter;

class TransmissionMonitor {
  public:
    bool isCounterBuiltin();
    bool isCounterShared();
    TransmissionCounter* getTransmissionCounter(bool createIfNotFound=true);
    void setTransmissionCounter(TransmissionCounter* counter, bool shared=true);
  private:
    TransmissionCounter* _counter = NULL;
    bool _counterBuiltin = false;
    bool _counterShared = true;
};

#endif
