#include "Rotation-Command-Resolver.h"

void RotationCommandResolver::onLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  leftDirection = 2;
  rightDirection = 1;
  leftSpeed = rightSpeed = int_abs(x);
}

void RotationCommandResolver::onRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  leftDirection = 1;
  rightDirection = 2;
  leftSpeed = rightSpeed = int_abs(x);
}
