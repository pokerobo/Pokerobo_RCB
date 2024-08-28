#ifndef __POKEROBO_RCB_ACCELEROMETER_ADXL345_H__
#define __POKEROBO_RCB_ACCELEROMETER_ADXL345_H__

#include "Accelerometer_Handler.h"

class ADXL345Handler: public AccelerometerHandler {
  public:
    void begin();
    void read();
    float getRoll();
    float getPitch();
  private:
    float X_out, Y_out, Z_out;
    float roll, pitch, rollF, pitchF=0;
};

#endif
