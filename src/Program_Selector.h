#ifndef __PROGRAM_SELECTOR_H__
#define __PROGRAM_SELECTOR_H__

#include "Display_Handler.h"
#include "Joystick_Handler.h"
#include "Program_Collection.h"
#include "Program_Definition.h"

#define PROGRAM_MODE_CLIENT                 2
#define PROGRAM_MODE_PLAYER                 5
#define PROGRAM_MODE_TESTER                 9

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

#ifndef __DEVMODE_PROGRAM_SELECTOR__
#define __DEVMODE_PROGRAM_SELECTOR__ 0
#endif//__DEVMODE_PROGRAM_SELECTOR__

class ProgramSelector {
  public:
    ProgramSelector();
    int begin(uint8_t mode=PROGRAM_MODE_PLAYER);
    void set(DisplayHandler* displayHandler);
    void set(JoystickHandler* joystickHandler);
    bool add(ProgramCapsule* programCapsule);
    int check();
#if __DEVMODE_PROGRAM_SELECTOR__
    void showMenu();
#endif
  protected:
    int wait_(int state);
  private:
    uint8_t _mode = PROGRAM_MODE_PLAYER;
    ProgramCollection* _programCollection = NULL;
    DisplayHandler* _displayHandler = NULL;
    JoystickHandler* _joystickHandler = NULL;
    int move_();
};

#endif
