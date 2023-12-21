#ifndef __MESSAGE_RESOLVER_H__
#define __MESSAGE_RESOLVER_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Joystick_Action.h"

class CommandPacket: public MessageInterface {
  public:
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
};

class CommandResolver {
  public:
    virtual CommandPacket* resolve(CommandPacket* command, JoystickAction* action, int coeff=1, bool rotatable=false);
};

#endif
