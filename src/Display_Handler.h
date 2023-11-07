#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Sender.h"

class DisplayHandler: public MessageRenderer {
  public:
    int begin();
    void clear();
    void splash(char* title);
    bool render(JoystickAction* message, SpeedPacket* speedPacket=NULL, TransmissionCounter* counter=NULL);
  private:
    uint8_t _maxCharHeight = 10;
    uint8_t _maxCharWidth = 4;
};

#endif
