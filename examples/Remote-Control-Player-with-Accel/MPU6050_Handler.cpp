#include "MPU6050_Handler.h"

const int MPU = 0x68; // MPU6050 I2C address

void MPU6050Handler::begin() {
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  //*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  //*/
}

void MPU6050Handler::check() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  _acceX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  _acceY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  _acceZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  //
  float swap = _acceX; _acceX = _acceY; _acceY = -swap;
  //
  // Calculating Roll and Pitch from the accelerometer data
  _accAngleX = (atan(_acceY / sqrt(pow(_acceX, 2) + pow(_acceZ, 2))) * 180 / PI);
  _accAngleY = (atan(-1 * _acceX / sqrt(pow(_acceY, 2) + pow(_acceZ, 2))) * 180 / PI);

  roll = _accAngleX;
  pitch = _accAngleY;
}

float MPU6050Handler::getRoll() {
  return roll;
}

float MPU6050Handler::getPitch() {
  return pitch;
}
