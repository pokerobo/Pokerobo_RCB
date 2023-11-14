#ifndef __MESSAGE_RENDERER_H__
#define __MESSAGE_RENDERER_H__

#include "Message_Exchange.h"
#include "Moving_Command.h"

class MessageRenderer {
  public:
    virtual void clear();
    virtual void splash(char* title, byte align = 0);
    virtual void render(JoystickAction* message, MovingCommand* movingCommand=NULL, TransmissionCounter* counter=NULL);
};

class ConsoleMessageRenderer: public MessageRenderer {
  public:
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, MovingCommand* movingCommand=NULL, TransmissionCounter* counter=NULL);
};

#endif
