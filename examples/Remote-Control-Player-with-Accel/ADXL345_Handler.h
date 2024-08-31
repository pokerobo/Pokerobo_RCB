#ifndef __POKEROBO_RCB_ACCELEROMETER_ADXL345_H__
#define __POKEROBO_RCB_ACCELEROMETER_ADXL345_H__

#include "Accelerometer_Handler.h"

class ADXL345Handler: public AccelerometerHandler {
  public:
    void begin();
    void check();
    float getRoll();
    float getPitch();
  private:
    float _accelX, _accelY, _accelZ;
    float _accAngleX, _accAngleY;
    float _roll=0, _pitch=0;
};

#endif
