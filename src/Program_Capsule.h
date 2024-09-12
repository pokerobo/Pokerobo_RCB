#ifndef __POKEROBO_RCB_PROGRAM_CAPSULE_H__
#define __POKEROBO_RCB_PROGRAM_CAPSULE_H__

#include "Commons.h"

class ProgramCapsule {
  public:
    virtual uint8_t getId();
    virtual char* getTitle();
    virtual char* getTitle(char* buffer);
    virtual int begin();
    virtual int check(void* control, void* command=NULL);
    virtual int close();
    static int getMaxTitleLength();
  protected:
    static int _maxTitleLength;
};

#ifndef PROGRAM_TITLE_PARTS
#define PROGRAM_TITLE_PARTS         4
#endif//PROGRAM_TITLE_PARTS

class ProgramSticker: public ProgramCapsule {
  public:
    ProgramSticker(char* title);
    ProgramSticker(char* titles[PROGRAM_TITLE_PARTS]);
    char* getTitle();
    char* getTitle(char *buffer);
  private:
    void initialize(char* titles[PROGRAM_TITLE_PARTS]=NULL);
    int getTitleLength();
    char* _title = NULL;
    char* _titles[PROGRAM_TITLE_PARTS];
};

#endif
