#ifndef __POKEROBO_RCB_PROGRAM_SELECTOR_H__
#define __POKEROBO_RCB_PROGRAM_SELECTOR_H__

#include "Display_Handler.h"
#include "Joystick_Handler.h"
#include "Program_Collection.h"

#define SCREEN_FLOW_CONFIGURATION        1
#define SCREEN_FLOW_APPLICATION          2

#ifndef PROGRAM_MENU_TOGGLE_BUTTON
#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON
#endif//PROGRAM_MENU_TOGGLE_BUTTON

class ProgramSelector {
  public:
    ProgramSelector(DisplayHandler* displayHandler=NULL,
        JoystickHandler* joystickHandler=NULL,
        uint8_t initialMode = SCREEN_FLOW_APPLICATION);
    int begin();
    void setDelayAmount(int amount);
    void set(DisplayHandler* displayHandler);
    void set(JoystickHandler* joystickHandler);
    bool add(ProgramCapsule* programPointer);
    int check();
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
    int _delayAmount = 10;
    uint8_t _flow = SCREEN_FLOW_APPLICATION;
    ProgramCollection* _programCollection = NULL;
    DisplayHandler* _displayHandler = NULL;
    JoystickHandler* _joystickHandler = NULL;
    bool _blank = true;
};

#endif
