#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Message_Renderer.h"
#include "Program_Collection.h"

#define JOYSTICK_INFO_COLUMNS           8 // 7 chars and '\0'

class DisplayHandler: public MessageRenderer {
  public:
    DisplayHandler();
    int begin();
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
    void render(ProgramCollection* programCollection=NULL);
  protected:
    void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source, TransmissionCounter* counter);
    void renderDirectionState_(char *title, message_source_t source, TransmissionCounter* counter, uint8_t &_directionState, uint8_t &_directionTotal);
    void renderTitle_(uint8_t lx, uint8_t ty, char* title);
    void renderCoordinates_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, char lines[][JOYSTICK_INFO_COLUMNS]);
    void renderTransmissionCounter_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, TransmissionCounter* counter);
    void renderJoystickPad_(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
    virtual void renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket);
    void* _u8g2Ref = NULL;
  private:
    uint8_t _maxCharHeight = 8;
    uint8_t _maxCharWidth = 5;
    uint8_t _directionState = 0;
    uint8_t _directionTotal = 0;
};

#endif
