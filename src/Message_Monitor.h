#ifndef __POKEROBO_RCB_MESSAGE_MONITOR_H__
#define __POKEROBO_RCB_MESSAGE_MONITOR_H__

#include "Commons.h"

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
