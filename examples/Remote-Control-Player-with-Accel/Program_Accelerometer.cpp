#include "Program_Accelerometer.h"

int ProgramAccelerometer::check(void* action_, void* command=NULL) {
  getAcceGyroReader()->read();

  float roll = getAcceGyroReader()->getRoll();
  float pitch = getAcceGyroReader()->getPitch();

  uint16_t y = 0;
  if (roll < -70.0) {
    y = 0;
  } else if (roll >= -70.0 && roll <= -20.0) {
    y = map((int)roll, -70, -20, 0, 511);
  } else if (roll > -20.0 && roll < 20.0) {
    y = 512;
  } else if (roll >= 20.0 && roll <= 50.0) {
    y = map((int)roll, 20, 50, 512, 1023);
  } else {
    y = 1024;
  }

  uint16_t x = 0;
  pitch = -pitch;
  if (pitch < -70.0) {
    x = 0;
  } else if (pitch >= -70.0 && pitch <= -20.0) {
    x = map((int)pitch, -70, -20, 0, 511);
  } else if (pitch > -20.0 && pitch < 20.0) {
    x = 512;
  } else if (pitch >= 20.0 && pitch <= 70.0) {
    x = map((int)pitch, 20, 70, 512, 1023);
  } else {
    x = 1024;
  }

  JoystickAction* action = (JoystickAction*) action_;
  action->update(x, y);

  return ProgramTransmitter::check(action, command);
}

AcceGyroReader* ProgramAccelerometer::getAcceGyroReader() {
  if (_acceGyroReader == NULL) {
    _acceGyroReader = new AcceGyroReader();
  }
  return _acceGyroReader;
}
