#ifndef __POKEROBO_RCB_PROGRAM_CAPSULE_H__
#define __POKEROBO_RCB_PROGRAM_CAPSULE_H__

#include "Commons.h"

class ProgramCapsule {
  public:
    virtual uint8_t getId();
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* control, void* command=NULL);
    virtual int close();
};

class ProgramWrapper: ProgramCapsule {
  public:
    virtual char* getTitle();
};

#endif
