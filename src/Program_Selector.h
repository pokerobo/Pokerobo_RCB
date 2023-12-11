#ifndef __PROGRAM_SELECTOR_H__
#define __PROGRAM_SELECTOR_H__

#include "Commons.h"
#include "Joystick_Handler.h"
#include "Program_Collections.h"

#ifndef PROGRAM_CAPSULES_LIMIT
#define PROGRAM_CAPSULES_LIMIT              20
#endif//PROGRAM_CAPSULES_LIMIT

#define PROGRAM_MODE_CLIENT                 2
#define PROGRAM_MODE_PLAYER                 5
#define PROGRAM_MODE_TESTER                 9

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

class ProgramSelector {
  public:
    int begin(uint8_t mode=PROGRAM_MODE_PLAYER);
    void set(MessageRenderer* messageRenderer);
    void set(JoystickHandler* joystickHandler);
    bool add(ProgramCapsule* programCapsule);
    int check();
  protected:
    int wait_(int state);
  private:
    uint8_t _mode = PROGRAM_MODE_PLAYER;
    ProgramCapsule* _programCapsules[PROGRAM_CAPSULES_LIMIT] = {};
    uint8_t _programCapsulesTotal = 0;
    uint8_t _programIndex = PROGRAM_CAPSULES_LIMIT;
    MessageRenderer* _messageRenderer = NULL;
    JoystickHandler* _joystickHandler = NULL;
    int move_();
};

#endif
