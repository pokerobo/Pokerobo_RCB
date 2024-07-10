#include "Execution_Timer.h"

void ExecutionTimer::start() {
  _beginTime = millis();
  _endTime = 0;
}

uint32_t ExecutionTimer::finish() {
  if (_endTime == 0) {
    _endTime = millis();
  }
  return _endTime - _beginTime;
}

uint32_t ExecutionTimer::remain(uint32_t stepTime) {
  uint32_t elapsedTime = finish();
  return (stepTime > elapsedTime) ? stepTime - elapsedTime : 0;
}
