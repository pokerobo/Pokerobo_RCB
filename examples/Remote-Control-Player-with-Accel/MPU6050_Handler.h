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
    float AccX, AccY, AccZ;
    float accAngleX, accAngleY;
    float roll, pitch;
};

#endif
