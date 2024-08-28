#ifndef __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__
#define __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__

#include <Pokerobo_RCB.h>
#include "MPU6050_Reader.h"

class ProgramAccelerometer: public ProgramTransmitter {
  public:
    using ProgramTransmitter::ProgramTransmitter;
    int check(void* action, void* command=NULL);
  protected:
    AcceGyroReader* getAcceGyroReader();
  private:
    AcceGyroReader* _acceGyroReader = NULL;
};

#endif
