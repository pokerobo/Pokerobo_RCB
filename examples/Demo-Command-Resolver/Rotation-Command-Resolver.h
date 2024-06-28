#ifndef __POKEROBO_RCB_ROTATION_COMMAND_RESOLVER_H__
#define __POKEROBO_RCB_ROTATION_COMMAND_RESOLVER_H__

#include <Pokerobo_RCB.h>

class RotationCommandResolver: public MovingCommandResolver {
  protected:
    void onLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    void onRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
};

#endif
