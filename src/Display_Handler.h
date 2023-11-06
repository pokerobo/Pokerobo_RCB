#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"

class DisplayHandler: public MessageRenderer {
  public:
    DisplayHandler();
    int begin();
    void clear();
    bool render(JoystickAction* message);
    bool render(JoystickAction* message, SpeedPacket* speedPacket, TransmissionCounter* counter=NULL);
  private:
    uint8_t _maxCharHeight = 10;
    uint8_t _maxCharWidth = 4;
    uint8_t _virtualPadOx = 64;
};

#endif
