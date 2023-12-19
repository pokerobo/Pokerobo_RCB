#ifndef __MESSAGE_PROCESSOR_H__
#define __MESSAGE_PROCESSOR_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Joystick_Action.h"

class MessageProcessor {
  public:
    virtual int process(JoystickAction* action, MessageInterface* commandPacket);
};

#endif
