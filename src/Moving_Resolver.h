#ifndef __SPEED_RESOLVER_H__
#define __SPEED_RESOLVER_H__

#include "Commons.h"
#include "Message_Exchange.h"

class MovingResolver {
  public:
    MovingCommand* resolve(MovingCommand* command, JoystickAction* action, int coeff=1, bool rotatable=false);
};

#endif
