#ifndef __SPEED_RESOLVER_H__
#define __SPEED_RESOLVER_H__

#include "Commons.h"
#include "Message_Exchange.h"

class SpeedResolver {
  public:
    SpeedPacket* resolve(SpeedPacket* packet, JoystickAction* action, int coeff=1, bool rotatable=false);
};

#endif
