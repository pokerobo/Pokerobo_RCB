#ifndef __POKEROBO_RCB_MESSAGE_PROCESSOR_H__
#define __POKEROBO_RCB_MESSAGE_PROCESSOR_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Joystick_Action.h"

class MessageProcessor {
  public:
    virtual int process(MasterContext* context, JoystickAction* control, MessageInterface* command);
};

#endif
