#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"

class DisplayHandler: public MessageRenderer {
  public:
    DisplayHandler();
    int begin();
    bool render(JoystickAction* message);
  private:
    int _maxCharHeight = 10;
    int _maxCharWidth = 4;
    int _virtualPadOx = 64;
};

#endif
