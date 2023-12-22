#ifndef __PROGRAM_SELECTOR_H__
#define __PROGRAM_SELECTOR_H__

#include "Display_Handler.h"
#include "Joystick_Handler.h"
#include "Program_Collection.h"

#define PROGRAM_MODE_CLIENT                 2
#define PROGRAM_MODE_PLAYER                 5
#define PROGRAM_MODE_TESTER                 9

#define DASHBOARD_FLOW_CONFIGURATION        1
#define DASHBOARD_FLOW_EXECUTION            2

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

#ifndef __DEVMODE_PROGRAM_SELECTOR__
#define __DEVMODE_PROGRAM_SELECTOR__        0
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
    int move_();
    int enterDashboard_(JoystickAction* action);
    int processDashboard_(JoystickAction* action);
    int leaveDashboard_(JoystickAction* action);
    int enterProgram_(JoystickAction* action);
    int executeProgram_(JoystickAction* action);
    int leaveProgram_(JoystickAction* action);
    void changeFlow_(uint8_t flow);
  private:
    uint8_t _mode = PROGRAM_MODE_PLAYER;
    uint8_t _flow = DASHBOARD_FLOW_EXECUTION;
    ProgramCollection* _programCollection = NULL;
    DisplayHandler* _displayHandler = NULL;
    JoystickHandler* _joystickHandler = NULL;
};

#endif
