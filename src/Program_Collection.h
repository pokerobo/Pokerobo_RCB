#ifndef __POKEROBO_RCB_PROGRAM_COLLECTION_H__
#define __POKEROBO_RCB_PROGRAM_COLLECTION_H__

#include "Program_Capsule.h"

#ifndef PROGRAM_COLLECTION_LIMIT
#define PROGRAM_COLLECTION_LIMIT              20
#endif//PROGRAM_COLLECTION_LIMIT

class ProgramCollection {
  public:
    bool isReady();
    uint8_t getTotal();
    uint8_t getCurrentIndex();
    bool setCurrentIndex(uint8_t j);
    uint8_t getFocusIndex();
    bool setFocusIndex(uint8_t j);
    bool setFocusAsCurrent();
    bool moveFocusUp();
    bool moveFocusDown();
    void decreaseItemIndex();
    void increaseItemIndex();
    ProgramCapsule* getItem(uint8_t i);
    ProgramCapsule* getCurrentItem();
    bool add(ProgramCapsule* programPointer);
    uint8_t getFrameBegin();
    bool setFrameBegin(uint8_t pos);
    uint8_t getFrameEnd();
    bool setFrameEnd(uint8_t pos);
    static const uint8_t frameHeight;
  private:
    ProgramCapsule* _programPointers[PROGRAM_COLLECTION_LIMIT] = {};
    uint8_t _programPointersTotal = 0;
    uint8_t _programIndex = PROGRAM_COLLECTION_LIMIT;
    uint8_t _focusIndex = PROGRAM_COLLECTION_LIMIT;
    uint8_t _frameBegin = 0;
};

#endif
