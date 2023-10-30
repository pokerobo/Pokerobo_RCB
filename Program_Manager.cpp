#include "Program_Manager.h"

void ProgramManager::start() {
  _beginTime = millis();
}

uint32_t ProgramManager::finish() {
  return (_endTime = millis()) - _beginTime;
}

uint32_t ProgramManager::remain(uint32_t stepTime) {
  return max(stepTime - (_endTime - _beginTime), 0);
}
