#ifndef __PROGRAM_COLLECTION_H__
#define __PROGRAM_COLLECTION_H__

#include "Program_Capsule.h"

#ifndef PROGRAM_CAPSULES_LIMIT
#define PROGRAM_CAPSULES_LIMIT              20
#endif//PROGRAM_CAPSULES_LIMIT

class ProgramCollection {
  public:
    bool isReady();
    uint8_t getTotal();
    uint8_t getCurrentIndex();
    bool setCurrentIndex(uint8_t j);
    uint8_t getFocusIndex();
    bool setFocusIndex(uint8_t j);
    bool setFocusAsCurrent();
    ProgramCapsule* getItem(uint8_t i);
    ProgramCapsule* getCurrentItem();
    bool add(ProgramCapsule* programCapsule);
    uint8_t getFrameBegin();
    bool updateFrameBegin(uint8_t pos);
    uint8_t getFrameEnd();
    bool updateFrameEnd(uint8_t pos);
    static const uint8_t frameHeight;
  private:
    ProgramCapsule* _programCapsules[PROGRAM_CAPSULES_LIMIT] = {};
    uint8_t _programCapsulesTotal = 0;
    uint8_t _programIndex = PROGRAM_CAPSULES_LIMIT;
    uint8_t _focusIndex = PROGRAM_CAPSULES_LIMIT;
    uint8_t _frameBegin = 0;
};

#endif
