#ifndef __PROGRAM_CAPSULE_H__
#define __PROGRAM_CAPSULE_H__

#include "Commons.h"

class ProgramCapsule {
  public:
    virtual uint8_t getId();
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* action = NULL);
    virtual int close();
};

#endif
