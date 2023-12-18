#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Message_Renderer.h"
#include "Program_Collection.h"

class DisplayHandler: public MessageRenderer {
  public:
    int begin();
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, MovingCommand* movingCommand=NULL, TransmissionCounter* counter=NULL);
    void render(ProgramCollection* programCollection=NULL);
  protected:
    virtual void renderCommandPacket_(uint8_t lx, uint8_t ty, MovingCommand* movingCommand);
  private:
    uint8_t _maxCharHeight = 8;
    uint8_t _maxCharWidth = 5;
};

class MovingDisplayHandler: public DisplayHandler {
  protected:
    void renderCommandPacket_(uint8_t lx, uint8_t ty, MovingCommand* movingCommand);
};

#endif
