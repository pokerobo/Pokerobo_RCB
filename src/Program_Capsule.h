#ifndef __POKEROBO_RCB_PROGRAM_CAPSULE_H__
#define __POKEROBO_RCB_PROGRAM_CAPSULE_H__

#include "Commons.h"

#define PROGRAM_TYPE_1              1
#define PROGRAM_TYPE_2              2

class ProgramCapsule {
  public:
    virtual bool isTypeOf(byte label);
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
    bool isTypeOf(byte label);
    char* getTitle();
    char* getTitle(char *buffer);
  private:
    void initialize(char* titles[PROGRAM_TITLE_PARTS]=NULL);
    int getTitleLength();
    char* _title = NULL;
    char* _titles[PROGRAM_TITLE_PARTS];
};

class ProgramPagelet: public ProgramCapsule {
  public:
    ProgramPagelet(char* titleOrTemplate,
        uint16_t minIndex=0, uint16_t maxIndex=0xff, uint16_t index=1);
    bool isTypeOf(byte label);
    char* getTitle(char *buffer);
    uint16_t getIndex();
    void next();
    void prev();
    bool hasChanged();
    bool change();
  protected:
    virtual void onChanged(uint16_t currentIndex, uint16_t currentFocus);
  private:
    int getTitleLength();
    uint16_t _minIndex = 0;
    uint16_t _maxIndex = 0xff;
    uint16_t _currentIndex = 1;
    uint16_t _currentFocus = 1;
    char* _titleTemplate = NULL;
};

#endif
