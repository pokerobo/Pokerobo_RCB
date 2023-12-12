#ifndef __PROGRAM_CAPSULE_H__
#define __PROGRAM_CAPSULE_H__

#include "Commons.h"
#include "Joystick_Action.h"

class ProgramCapsule {
  public:
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* action = NULL);
    virtual int close();
};

#endif
