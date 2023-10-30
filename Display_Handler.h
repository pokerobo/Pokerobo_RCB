#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"

class DisplayHandler: public MessageRenderer {
  public:
    DisplayHandler();
    int begin();
    bool render(JoystickAction* message);
  protected:
    bool print(char lines[][24]);
};

#endif
