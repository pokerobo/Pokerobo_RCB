#include "Program_Selector.h"

#ifndef __DEBUG_LOG_PROGRAM_SELECTOR__
#define __DEBUG_LOG_PROGRAM_SELECTOR__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_PROGRAM_SELECTOR__

ProgramCollection programCollection;

ProgramSelector::ProgramSelector(DisplayHandler* displayHandler,
    JoystickHandler* joystickHandler,
    uint8_t initialMode) {
  _displayHandler = displayHandler;
  _joystickHandler = joystickHandler;
  _programCollection = &programCollection;
  _flow = initialMode;
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
      case SCREEN_FLOW_APPLICATION:
        leaveProgram_(&message);
        changeFlow_(SCREEN_FLOW_CONFIGURATION);
        return enterDashboard_(&message);
      case SCREEN_FLOW_CONFIGURATION:
        leaveDashboard_(&message);
        changeFlow_(SCREEN_FLOW_APPLICATION);
        return enterProgram_(&message);
    }
  }

  switch(_flow) {
    case SCREEN_FLOW_CONFIGURATION:
      return processDashboard_(&message);
    case SCREEN_FLOW_APPLICATION:
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
  bool _refreshed = false;

  uint8_t toggle = action->getTogglingFlags() >> 12;
  if (toggle & 0b0001) { // LEFT -> BACK
    changeFlow_(SCREEN_FLOW_APPLICATION);
    return 0;
  }

  if (toggle & 0b0010) { // UP -> PREV
    _programCollection->moveFocusUp();
    _refreshed = true;
  } else
  if (toggle & 0b0100) { // DOWN -> NEXT
    _programCollection->moveFocusDown();
    _refreshed = true;
  } else
  if (toggle & 0b1000) { // RIGHT -> SELECT
    _programCollection->setFocusAsCurrent();
    _refreshed = true;
  }

  if (action->isButtonClicked(MASK_LEFT_BUTTON)) {
    _programCollection->decreaseItemIndex();
    _refreshed = true;
  } else
  if (action->isButtonClicked(MASK_RIGHT_BUTTON)) {
    _programCollection->increaseItemIndex();
    _refreshed = true;
  }

  if (_refreshed) {
    _displayHandler->render(_programCollection);
  }

  if (_blank) {
    _blank = false;
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

bool ProgramSelector::add(ProgramCapsule* programPointer) {
  return _programCollection->add(programPointer);
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
