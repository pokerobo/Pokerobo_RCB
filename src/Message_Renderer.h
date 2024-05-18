#ifndef __POKEROBO_RCB_MESSAGE_RENDERER_H__
#define __POKEROBO_RCB_MESSAGE_RENDERER_H__

#include "Message_Exchange.h"
#include "Joystick_Action.h"

class MessageRenderer {
  public:
    virtual void clear();
    virtual void splash(char* title, byte align = 0);
    virtual void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
};

#endif
