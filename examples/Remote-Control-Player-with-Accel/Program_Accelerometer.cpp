#include "Program_Accelerometer.h"

#define ROLL_NEGATIVE_MAX_RANGE     70
#define ROLL_NEGATIVE_MIN_RANGE     20
#define ROLL_POSITIVE_MIN_RANGE     20
#define ROLL_POSITIVE_MAX_RANGE     50
#define Y_FRONT_CUT_OFF             288
#define Y_BACK_CUT_OFF              256

#define PITCH_NEGATIVE_MAX_RANGE    70
#define PITCH_NEGATIVE_MIN_RANGE    20
#define PITCH_POSITIVE_MIN_RANGE    20
#define PITCH_POSITIVE_MAX_RANGE    70
#define X_CUT_OFF                   255

int ProgramAccelerometer::check(void* action_, void* command=NULL) {
  getAccelerometerHandler()->check();

  float roll = getAccelerometerHandler()->getRoll();
  float pitch = getAccelerometerHandler()->getPitch();

  uint16_t y = 0;
  if (roll < -ROLL_NEGATIVE_MAX_RANGE) {
    y = 0;
  } else if (roll >= -ROLL_NEGATIVE_MAX_RANGE && roll <= -ROLL_NEGATIVE_MIN_RANGE) {
    y = map((int)roll, -ROLL_NEGATIVE_MAX_RANGE, -ROLL_NEGATIVE_MIN_RANGE, 0, 511 - Y_BACK_CUT_OFF);
  } else if (roll > -ROLL_NEGATIVE_MIN_RANGE && roll < ROLL_POSITIVE_MIN_RANGE) {
    y = 512;
  } else if (roll >= ROLL_POSITIVE_MIN_RANGE && roll <= ROLL_POSITIVE_MAX_RANGE) {
    y = map((int)roll, ROLL_POSITIVE_MIN_RANGE, ROLL_POSITIVE_MAX_RANGE, 512 + Y_FRONT_CUT_OFF, 1023);
  } else {
    y = 1024;
  }

  uint16_t x = 0;
  pitch = -pitch;
  if (pitch < -PITCH_NEGATIVE_MAX_RANGE) {
    x = 0;
  } else if (pitch >= -PITCH_NEGATIVE_MAX_RANGE && pitch <= -PITCH_NEGATIVE_MIN_RANGE) {
    x = map((int)pitch, -PITCH_NEGATIVE_MAX_RANGE, -PITCH_NEGATIVE_MIN_RANGE, 0, 511 - X_CUT_OFF);
  } else if (pitch > -PITCH_NEGATIVE_MIN_RANGE && pitch < PITCH_POSITIVE_MIN_RANGE) {
    x = 512;
  } else if (pitch >= PITCH_POSITIVE_MIN_RANGE && pitch <= PITCH_POSITIVE_MAX_RANGE) {
    x = map((int)pitch, PITCH_POSITIVE_MIN_RANGE, PITCH_POSITIVE_MAX_RANGE, 512 + X_CUT_OFF, 1023);
  } else {
    x = 1024;
  }

  JoystickAction* action = (JoystickAction*) action_;
  action->update(x, y);

  return ProgramTransmitter::check(action_, command);
}

ProgramAccelerometer* ProgramAccelerometer::setAccelerometerHandler(AccelerometerHandler* handler) {
  _accelerometerHandler = handler;
  return this;
}

AccelerometerHandler* ProgramAccelerometer::getAccelerometerHandler() {
  return _accelerometerHandler;
}
