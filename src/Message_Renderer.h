#ifndef __MESSAGE_RENDERER_H__
#define __MESSAGE_RENDERER_H__

#include "Joystick_Action.h"
#include "Moving_Command.h"

class MessageRenderer {
  public:
    virtual void clear();
    virtual void splash(char* title, byte align = 0);
    virtual void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
};

#endif
