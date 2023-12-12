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
  if (!_programCollection->isReady()) {
    exit(1);
  }
  return 0;
}

int ProgramSelector::check() {
  return wait_(move_());
}

int ProgramSelector::wait_(int state) {
  delay(50);
  return state;
}

#if __DEVMODE_PROGRAM_SELECTOR__
int ProgramSelector::move_() {
  JoystickAction message;
  _joystickHandler->input(&message);

  uint16_t clickingFlags = message.getClickingFlags();
  if ((clickingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    switch(_flow) {
      case DASHBOARD_FLOW_EXECUTION:
        leaveProgram_(&message);
        _flow = DASHBOARD_FLOW_CONFIGURATION;
        return enterDashboard_(&message);
      case DASHBOARD_FLOW_CONFIGURATION:
        leaveDashboard_(&message);
        _flow = DASHBOARD_FLOW_EXECUTION;
        return enterProgram_(&message);
    }
  }

  switch(_flow) {
    case DASHBOARD_FLOW_CONFIGURATION:
      return processDashboard_(&message);
    case DASHBOARD_FLOW_EXECUTION:
      return executeProgram_(&message);
  }
}

int ProgramSelector::enterDashboard_(JoystickAction* action) {
  if (_displayHandler != NULL) {
    _displayHandler->render(_programCollection);
  }
  return 0;
}

int ProgramSelector::processDashboard_(JoystickAction* action) {
  return 0;
}

int ProgramSelector::leaveDashboard_(JoystickAction* action) {
  return 0;
}

int ProgramSelector::enterProgram_(JoystickAction* action) {
  return 0;
}

int ProgramSelector::executeProgram_(JoystickAction* action) {
  return _programCollection->getCurrentItem()->check(action);
}

int ProgramSelector::leaveProgram_(JoystickAction* action) {
  return 0;
}
#else
int ProgramSelector::move_() {
  JoystickAction message;
  _joystickHandler->input(&message);

  uint16_t clickingFlags = message.getClickingFlags();
  if (!(clickingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    return _programCollection->getCurrentItem()->check(&message);
  }

  if (_programCollection->getTotal() < 2) {
    return -1;
  }

  uint8_t programNextIndex = _programCollection->getCurrentIndex() + 1;
  if (programNextIndex >= _programCollection->getTotal()) {
    programNextIndex = 0;
  }
  ProgramCapsule* programNext = _programCollection->getItem(programNextIndex);
  ProgramCapsule* program = _programCollection->getCurrentItem();

  program->close();
  programNext->begin();

  _programCollection->setCurrentIndex(programNextIndex);

  return 0;
}
#endif

bool ProgramSelector::add(ProgramCapsule* programCapsule) {
  return _programCollection->add(programCapsule);
}

void ProgramSelector::set(DisplayHandler* displayHandler) {
  _displayHandler = displayHandler;
}

void ProgramSelector::set(JoystickHandler* joystickHandler) {
  _joystickHandler = joystickHandler;
}

#if __DEVMODE_PROGRAM_SELECTOR__
void ProgramSelector::showMenu() {
  if (_displayHandler == NULL) {
    return;
  }
  _displayHandler->render(_programCollection);
}
#endif
