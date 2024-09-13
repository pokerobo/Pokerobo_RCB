#include "Program_Collection.h"

bool ProgramCollection::isReady() {
  return _programPointersTotal > 0 && _programIndex < PROGRAM_COLLECTION_LIMIT;
}

uint8_t ProgramCollection::getTotal() {
  return _programPointersTotal;
}

uint8_t ProgramCollection::getCurrentIndex() {
  return _programIndex;
}

bool ProgramCollection::setCurrentIndex(uint8_t j) {
  if (j >= _programPointersTotal) {
    return false;
  }
  ProgramCapsule* currentItem = getItem(_programIndex);
  ProgramCapsule* newItem = getItem(j);
  if (j == _programIndex) {
    if (newItem != NULL && newItem->isTypeOf(PROGRAM_TYPE_2)) {
      ProgramPagelet* pointer = (ProgramPagelet*) newItem;
      if (!pointer->hasChanged()) {
        return true;
      }
    } else {
      return true;
    }
  }
  if (newItem->isTypeOf(PROGRAM_TYPE_2)) {
    ProgramPagelet* pointer = (ProgramPagelet*) newItem;
    pointer->change();
  }
  if (currentItem != NULL) {
    currentItem->close();
  }
  if (newItem != NULL) {
    newItem->begin();
  }
  _programIndex = j;
  return true;
}

uint8_t ProgramCollection::getFocusIndex() {
  return _focusIndex;
}

bool ProgramCollection::setFocusIndex(uint8_t j) {
  if (j >= _programPointersTotal) {
    return false;
  }
  if (j == _focusIndex) {
    return false;
  }
  if (j < getFrameBegin()) {
    setFrameBegin(j);
  }
  if (j > getFrameEnd()) {
    setFrameEnd(j);
  }
  _focusIndex = j;
  return true;
}

bool ProgramCollection::setFocusAsCurrent() {
  return setCurrentIndex(getFocusIndex());
}

bool ProgramCollection::moveFocusUp() {
  if (getFocusIndex() == 0) {
    return setFocusIndex(_programPointersTotal - 1);
  }
  return setFocusIndex(getFocusIndex() - 1);
}

bool ProgramCollection::moveFocusDown() {
  if (getFocusIndex() == (_programPointersTotal - 1)) {
    return setFocusIndex(0);
  }
  return setFocusIndex(getFocusIndex() + 1);
}

void ProgramCollection::decreaseItemIndex() {
  ProgramCapsule* capsule = getItem(getFocusIndex());
  if (!capsule->isTypeOf(PROGRAM_TYPE_2)) {
    return;
  }
  ProgramPagelet* pointer = (ProgramPagelet*) capsule;
  pointer->prev();
}

void ProgramCollection::increaseItemIndex() {
  ProgramCapsule* capsule = getItem(getFocusIndex());
  if (!capsule->isTypeOf(PROGRAM_TYPE_2)) {
    return;
  }
  ProgramPagelet* pointer = (ProgramPagelet*) capsule;
  pointer->next();
}

ProgramCapsule* ProgramCollection::getItem(uint8_t i) {
  if (i >= _programPointersTotal) {
    return NULL;
  }
  return _programPointers[i];
}

ProgramCapsule* ProgramCollection::getCurrentItem() {
  return getItem(getCurrentIndex());
}

bool ProgramCollection::add(ProgramCapsule* programPointer) {
  if (programPointer == NULL) {
    return false;
  }
  if (_programPointersTotal > PROGRAM_COLLECTION_LIMIT) {
    return false;
  }
  for(int i=0; i<_programPointersTotal; i++) {
    if (_programPointers[i] == programPointer) {
      return false;
    }
  }
  _programPointers[_programPointersTotal++] = programPointer;
  if (_programPointersTotal == 1) {
    setFrameBegin(0);
    setCurrentIndex(0);
    setFocusIndex(0);
  }
  return true;
}

const uint8_t ProgramCollection::frameHeight = 5;

uint8_t ProgramCollection::getFrameBegin() {
  return _frameBegin;
}

bool ProgramCollection::setFrameBegin(uint8_t pos) {
  if (pos >= _programPointersTotal) {
    return false;
  }
  _frameBegin = pos;
  return true;
}

uint8_t ProgramCollection::getFrameEnd() {
  uint8_t frameEnd = _frameBegin + frameHeight - 1;
  if (frameEnd >= _programPointersTotal) {
    frameEnd = _programPointersTotal - 1;
  }
  return frameEnd;
}

bool ProgramCollection::setFrameEnd(uint8_t pos) {
  if (pos >= _programPointersTotal) {
    return false;
  }
  uint8_t frameEnd = _frameBegin + frameHeight - 1;
  if (pos == frameEnd) {
    return false;
  }
  if (pos < frameHeight - 1) {
    _frameBegin = 0;
  } else {
    _frameBegin = pos - frameHeight + 1;
  }
  return true;
}
