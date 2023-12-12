#include "Program_Collection.h"

bool ProgramCollection::isReady() {
  return _programCapsulesTotal > 0 && _programIndex < PROGRAM_CAPSULES_LIMIT;
}

uint8_t ProgramCollection::getTotal() {
  return _programCapsulesTotal;
}

uint8_t ProgramCollection::getCurrentIndex() {
  return _programIndex;
}

bool ProgramCollection::setCurrentIndex(uint8_t j) {
  if (j >= _programCapsulesTotal) {
    return false;
  }
  if (j != _programIndex) {
    _programIndex = j;
  }
  return true;
}

uint8_t ProgramCollection::getFocusIndex() {
  return _focusIndex;
}

bool ProgramCollection::setFocusIndex(uint8_t j) {
  if (j >= _programCapsulesTotal) {
    return false;
  }
  if (j != _focusIndex) {
    _focusIndex = j;
  }
  return true;
}

bool ProgramCollection::setFocusAsCurrent() {
  return setCurrentIndex(_focusIndex);
}

ProgramCapsule* ProgramCollection::getItem(uint8_t i) {
  if (i >= _programCapsulesTotal) {
    return NULL;
  }
  return _programCapsules[i];
}

ProgramCapsule* ProgramCollection::getCurrentItem() {
  return getItem(getCurrentIndex());
}

bool ProgramCollection::add(ProgramCapsule* programCapsule) {
  if (programCapsule == NULL) {
    return false;
  }
  if (_programCapsulesTotal > PROGRAM_CAPSULES_LIMIT) {
    return false;
  }
  for(int i=0; i<_programCapsulesTotal; i++) {
    if (_programCapsules[i] == programCapsule) {
      return false;
    }
  }
  _programCapsules[_programCapsulesTotal++] = programCapsule;
  if (_programCapsulesTotal == 1) {
    _programIndex = 0;
  }
  return true;
}

const uint8_t ProgramCollection::frameHeight = 5;

uint8_t ProgramCollection::getFrameBegin() {
  return _frameBegin;
}

bool ProgramCollection::updateFrameBegin(uint8_t pos) {
  if (pos >= _programCapsulesTotal) {
    return false;
  }
  _frameBegin = pos;
  return true;
}

uint8_t ProgramCollection::getFrameEnd() {
  uint8_t frameEnd = _frameBegin + frameHeight - 1;
  if (frameEnd >= _programCapsulesTotal) {
    return _programCapsulesTotal - 1;
  }
  return frameEnd;
}

bool ProgramCollection::updateFrameEnd(uint8_t pos) {
  if (pos >= _programCapsulesTotal) {
    return false;
  }
  if (pos < frameHeight - 1) {
    _frameBegin = 0;
  } else {
    _frameBegin = pos - frameHeight + 1;
  }
  return true;
}
