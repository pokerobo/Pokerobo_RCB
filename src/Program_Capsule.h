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

#define   PROGRAM_TITLE_PARTS         3

class ProgramSticker: public ProgramCapsule {
  public:
    ProgramSticker(char* title);
    ProgramSticker(char* titles[]);
    char* getTitle();
    char* getTitle(char *buffer);
    int getTitleLength();
  private:
    void initialize();
    void initialize(char* titles[]);
    char* _title = NULL;
    char* _titles[PROGRAM_TITLE_PARTS];
};

#endif
