#include "Program_Selector.h"

#ifndef __DEBUG_LOG_PROGRAM_SELECTOR__
#define __DEBUG_LOG_PROGRAM_SELECTOR__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_PROGRAM_SELECTOR__

ProgramCollection programCollection;

ProgramSelector::ProgramSelector(DisplayHandler* displayHandler,
    JoystickHandler* joystickHandler) {
  _displayHandler = displayHandler;
  _joystickHandler = joystickHandler;
  _programCollection = &programCollection;
}

int ProgramSelector::begin() {
  if (!_programCollection->isReady()) {
    exit(1);
  }
  return 0;
}

int ProgramSelector::check() {
  return wait_(move_());
}

int ProgramSelector::wait_(int state) {
  if (_delayAmount > 0) {
    delay(_delayAmount);
  }
  return state;
}

int ProgramSelector::move_() {
  JoystickAction message;
  _joystickHandler->input(&message);

  uint16_t togglingFlags = message.getTogglingFlags();
  if ((togglingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    switch(_flow) {
      case DASHBOARD_FLOW_EXECUTION:
        leaveProgram_(&message);
        changeFlow_(DASHBOARD_FLOW_CONFIGURATION);
        return enterDashboard_(&message);
      case DASHBOARD_FLOW_CONFIGURATION:
        leaveDashboard_(&message);
        changeFlow_(DASHBOARD_FLOW_EXECUTION);
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
  uint8_t toggle = action->getTogglingFlags() >> 12;
  if (toggle & 0b0001) { // LEFT -> BACK
    changeFlow_(DASHBOARD_FLOW_EXECUTION);
  } else
  if (toggle & 0b0010) { // UP -> PREV
    _programCollection->moveFocusUp();
    _displayHandler->render(_programCollection);
  } else
  if (toggle & 0b0100) { // DOWN -> NEXT
    _programCollection->moveFocusDown();
    _displayHandler->render(_programCollection);
  } else
  if (toggle & 0b1000) { // RIGHT -> SELECT
    _programCollection->setFocusAsCurrent();
    _displayHandler->render(_programCollection);
  }
  return 0;
}

int ProgramSelector::leaveDashboard_(JoystickAction* action) {
  _programCollection->setFocusAsCurrent();
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

void ProgramSelector::changeFlow_(uint8_t flow) {
  if (flow == _flow) return;
  _flow = flow;
  _displayHandler->clear();
}

bool ProgramSelector::add(ProgramCapsule* programCapsule) {
  return _programCollection->add(programCapsule);
}

void ProgramSelector::set(DisplayHandler* displayHandler) {
  _displayHandler = displayHandler;
}

void ProgramSelector::set(JoystickHandler* joystickHandler) {
  _joystickHandler = joystickHandler;
}

void ProgramSelector::setDelayAmount(int amount) {
  _delayAmount = amount;
}

#if __DEVMODE_PROGRAM_SELECTOR__
void ProgramSelector::showMenu() {
  if (_displayHandler == NULL) {
    return;
  }
  _displayHandler->render(_programCollection);
}
#endif
