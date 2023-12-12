#include "Program_Selector.h"

#ifndef __DEBUG_LOG_PROGRAM_SELECTOR__
#define __DEBUG_LOG_PROGRAM_SELECTOR__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_PROGRAM_SELECTOR__

ProgramCollection programCollection;

ProgramSelector::ProgramSelector() {
  _programCollection = &programCollection;
}

int ProgramSelector::begin(uint8_t mode) {
  _mode = mode;
  return 0;
}

int ProgramSelector::check() {
  return wait_(move_());
}

int ProgramSelector::wait_(int state) {
  delay(50);
  return state;
}

int ProgramSelector::move_() {
  if (!_programCollection->isReady()) {
    exit(1);
  }

  JoystickAction message;
  _joystickHandler->input(&message);

  ProgramCapsule* program = _programCollection->getCurrentItem();

  uint16_t clickingFlags = message.getClickingFlags();
  if (!(clickingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    return program->check(&message);
  }

  if (_programCollection->getTotal() < 2) {
    return -1;
  }

  uint8_t programNextIndex = _programCollection->getCurrentIndex() + 1;
  if (programNextIndex >= _programCollection->getTotal()) {
    programNextIndex = 0;
  }
  ProgramCapsule* programNext = _programCollection->getItem(programNextIndex);

  program->close();
  programNext->begin();

  _programCollection->setCurrentIndex(programNextIndex);

  return 0;
}

bool ProgramSelector::add(ProgramCapsule* programCapsule) {
  return _programCollection->add(programCapsule);
}

void ProgramSelector::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

void ProgramSelector::set(JoystickHandler* joystickHandler) {
  _joystickHandler = joystickHandler;
}
