#ifndef __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__
#define __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__

#include <Pokerobo_RCB.h>
#include "Accelerometer_Handler.h"

class ProgramAccelerometer: public ProgramTransmitter {
  public:
    using ProgramTransmitter::ProgramTransmitter;
    int check(void* action, void* command=NULL);
    ProgramAccelerometer* setAccelerometerHandler(AccelerometerHandler* handler);
  protected:
    AccelerometerHandler* getAccelerometerHandler();
  private:
    AccelerometerHandler* _accelerometerHandler = NULL;
};

#endif
