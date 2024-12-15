#ifndef __POKEROBO_RCB_ACCELEROMETER_MPU6050_H__
#define __POKEROBO_RCB_ACCELEROMETER_MPU6050_H__

#include "Accelerometer_Handler.h"

class MPU6050Handler: public AccelerometerHandler {
  public:
    void begin();
    void check();
    float getRoll();
    float getPitch();
  private:
    float _acceX, _acceY, _acceZ;
    float _accAngleX, _accAngleY;
    float roll = 0, pitch = 0;
};

#endif
