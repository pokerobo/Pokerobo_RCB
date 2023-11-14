#ifndef __SPEED_RESOLVER_H__
#define __SPEED_RESOLVER_H__

#include "Commons.h"
#include "Joystick_Action.h"
#include "Moving_Command.h"

class MovingResolver {
  public:
    MovingCommand* resolve(MovingCommand* command, JoystickAction* action, int coeff=1, bool rotatable=false);
};

#endif
