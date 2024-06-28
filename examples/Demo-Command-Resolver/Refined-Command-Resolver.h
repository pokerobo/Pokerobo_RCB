#ifndef __POKEROBO_RCB_REFINED_COMMAND_RESOLVER_H__
#define __POKEROBO_RCB_REFINED_COMMAND_RESOLVER_H__

#include <Pokerobo_RCB.h>

class RefinedCommandResolver: public MovingCommandResolver {
  public:
    CommandPacket* resolve(CommandPacket* command, JoystickAction* action);
};

#endif
