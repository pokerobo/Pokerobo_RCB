#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__

#include "Commons.h"

class ProgramManager {
  public:
    void start();
    uint32_t finish();
    uint32_t remain(uint32_t stepTime=100);
  private:
    uint32_t _beginTime;
    uint32_t _endTime;
};

#endif
