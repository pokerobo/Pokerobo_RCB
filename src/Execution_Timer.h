#ifndef __POKEROBO_RCB_EXECUTION_TIMER_H__
#define __POKEROBO_RCB_EXECUTION_TIMER_H__

#include "Commons.h"

class ExecutionTimer {
  public:
    void start();
    uint32_t finish();
    uint32_t remain(uint32_t stepTime=100);
  private:
    uint32_t _beginTime;
    uint32_t _endTime;
};

#endif
