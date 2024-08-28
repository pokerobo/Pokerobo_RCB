#ifndef __POKEROBO_RCB_ACCE_GYRO_READER_H__
#define __POKEROBO_RCB_ACCE_GYRO_READER_H__

#include <Arduino.h>
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address

class AcceGyroReader {
  public:
    void begin();
    void read();
    float getRoll();
    float getPitch();
  private:
    float AccX, AccY, AccZ;
    float accAngleX, accAngleY;
    float roll, pitch;
};

#endif
