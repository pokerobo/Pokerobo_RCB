#ifndef __POKEROBO_RCB_MESSAGE_RESOLVER_H__
#define __POKEROBO_RCB_MESSAGE_RESOLVER_H__

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
    virtual CommandPacket* create();
    virtual CommandPacket* resolve(CommandPacket* command, JoystickAction* action);
    virtual void release(CommandPacket* command);
};

#endif
