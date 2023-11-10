#include "Execution_Timer.h"

void ExecutionTimer::start() {
  _beginTime = millis();
}

uint32_t ExecutionTimer::finish() {
  return (_endTime = millis()) - _beginTime;
}

uint32_t ExecutionTimer::remain(uint32_t stepTime) {
  return int_max(stepTime - (_endTime - _beginTime), 0);
}
