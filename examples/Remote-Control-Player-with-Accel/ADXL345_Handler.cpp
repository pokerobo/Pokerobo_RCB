#include "ADXL345_Handler.h"

const int ADXL345 = 0x53; // The ADXL345 sensor I2C address

void ADXL345Handler::begin() {
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // Bit D3 High for measuring enable (8dec -> 0000 1000 binary)
  Wire.endTransmission();
  delay(10);

  //Off-set Calibration
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);

  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);

  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);
}

void ADXL345Handler::check() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  _accelX = ( Wire.read() | Wire.read() << 8); // X-axis value
  _accelX = _accelX / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  _accelY = ( Wire.read() | Wire.read() << 8); // Y-axis value
  _accelY = _accelY / 256;
  _accelZ = ( Wire.read() | Wire.read() << 8); // Z-axis value
  _accelZ = _accelZ / 256;

  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  _accAngleX = atan(_accelY / sqrt(pow(_accelX, 2) + pow(_accelZ, 2))) * 180 / PI;
  _accAngleY = atan(-1 * _accelX / sqrt(pow(_accelY, 2) + pow(_accelZ, 2))) * 180 / PI;

  // Low-pass filter
  _roll = 0.94 * _roll + 0.06 * _accAngleX;
  _pitch = 0.94 * _pitch + 0.06 * _accAngleY;
}

float ADXL345Handler::getRoll() {
  return -_roll;
}

float ADXL345Handler::getPitch() {
  return -_pitch;
}
