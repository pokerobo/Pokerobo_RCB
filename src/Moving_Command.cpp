#include "Moving_Command.h"

SpeedPacket::SpeedPacket(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  update(leftSpeed, leftDirection, rightSpeed, rightDirection);
}

void SpeedPacket::update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  _LeftSpeed = leftSpeed;
  _LeftDirection = leftDirection;
  _RightSpeed = rightSpeed;
  _RightDirection = rightDirection;
}

int SpeedPacket::getLeftSpeed() {
  return _LeftSpeed;
}

byte SpeedPacket::getLeftDirection() {
  return _LeftDirection;
}

int SpeedPacket::getRightSpeed() {
  return _RightSpeed;
}

byte SpeedPacket::getRightDirection() {
  return _RightDirection;
}

const uint8_t SpeedPacket::messageSize = sizeof(uint8_t) +
    sizeof(uint8_t) +
    sizeof(uint8_t);

uint8_t SpeedPacket::length() {
  return messageSize;
}

uint8_t* SpeedPacket::serialize(uint8_t* buf, uint8_t len) {
  if (len < messageSize) {
    return NULL;
  }

  uint8_t directionFlags = 0;

  switch(_LeftDirection) {
    case 1:
      directionFlags |= 0b0001;
      break;
    case 2:
      directionFlags |= 0b0010;
      break;
  }

  switch(_RightDirection) {
    case 1:
      directionFlags |= 0b0100;
      break;
    case 2:
      directionFlags |= 0b1000;
      break;
  }

  buf[0] = directionFlags;
  buf[1] = _LeftSpeed;
  buf[2] = _RightSpeed;

  return buf;
}
