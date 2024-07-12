#ifndef __POKEROBO_RCB_MESSAGE_MONITOR_H__
#define __POKEROBO_RCB_MESSAGE_MONITOR_H__

#include "Commons.h"

typedef struct _TransmissionCounter {
  uint32_t baselineNumber = 0;
  uint32_t ordinalNumber = 0;
  uint32_t continualLossCount = 0;
  uint32_t packetLossTotal = 0;
  void adjust() {
    if (this->ordinalNumber >= 999999UL) {
      this->reset();
    }
  }
  void reset() {
    this->ordinalNumber = 0;
    this->continualLossCount = 0;
    this->packetLossTotal = 0;
  }
  void update(uint32_t count) {
    if (this->ordinalNumber == 0) {
      this->baselineNumber = count;
      this->packetLossTotal = 0;
    } else {
      if (count < this->ordinalNumber + 1) {
        this->baselineNumber = count;
        this->packetLossTotal = 0;
      } else if (count == this->ordinalNumber + 1) {
      } else if (count > this->ordinalNumber + 1) {
        this->packetLossTotal += count - this->ordinalNumber - 1;
      }
    }
    this->ordinalNumber = count;
  }
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
