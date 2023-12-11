#include "Program_Selector.h"

#ifndef __DEBUG_LOG_PROGRAM_SELECTOR__
#define __DEBUG_LOG_PROGRAM_SELECTOR__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_PROGRAM_SELECTOR__

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
  if (_programCapsulesTotal == 0 || _programIndex == PROGRAM_CAPSULES_LIMIT) {
    exit(1);
  }

  JoystickAction message;
  _joystickHandler->input(&message);

  ProgramCapsule* program = _programCapsules[_programIndex];

  uint16_t clickingFlags = message.getClickingFlags();
  if (!(clickingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    return program->check(&message);
  }

  if (_programCapsulesTotal < 2) {
    return -1;
  }

  uint8_t programNextIndex = _programIndex + 1;
  if (programNextIndex >= _programCapsulesTotal) {
    programNextIndex = 0;
  }
  ProgramCapsule* programNext = _programCapsules[programNextIndex];

  program->close();
  programNext->begin();

  _programIndex = programNextIndex;

  return 0;
}

bool ProgramSelector::add(ProgramCapsule* programCapsule) {
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

void ProgramSelector::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

void ProgramSelector::set(JoystickHandler* joystickHandler) {
  _joystickHandler = joystickHandler;
}
