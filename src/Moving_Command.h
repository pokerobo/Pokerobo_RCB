#ifndef __MOVING_COMMAND_H__
#define __MOVING_COMMAND_H__

#include "Commons.h"

class MovingCommand {
  public:
    static const uint8_t messageSize;
    MovingCommand(int leftSpeed=0, byte leftDirection=0, int rightSpeed=0, byte rightDirection=0);
    void update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection);
    int getLeftSpeed();
    byte getLeftDirection();
    int getRightSpeed();
    byte getRightDirection();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
  private:
    int _LeftSpeed;
    byte _LeftDirection;
    int _RightSpeed;
    byte _RightDirection;
};

#endif
