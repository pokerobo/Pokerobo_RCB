#ifndef __SPEED_RESOLVER_H__
#define __SPEED_RESOLVER_H__

#include "Commons.h"
#include "Display_Handler.h"
#include "Joystick_Action.h"
#include "Message_Exchange.h"
#include "Message_Serializer.h"
#include "Message_Renderer.h"
#include "Moving_Command.h"

class MovingCommandResolver {
  public:
    MovingCommand* resolve(MovingCommand* command, JoystickAction* action, int coeff=1, bool rotatable=false);
};

class MovingMessageSerializer: public MessageSerializer {
  public:
    uint8_t getSize();
    int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};

class MovingDisplayHandler: public DisplayHandler {
  protected:
    void renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket);
};

class MovingSerialConsole: public MessageRenderer {
  public:
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
};

#endif
