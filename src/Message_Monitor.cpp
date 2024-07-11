#include "Message_Monitor.h"

bool TransmissionMonitor::isCounterBuiltin() {
  return _counterBuiltin;
}

bool TransmissionMonitor::isCounterShared() {
  return _counterShared;
}

TransmissionCounter* TransmissionMonitor::getTransmissionCounter() {
  if (_counter == NULL) {
    _counter = new TransmissionCounter();
    _counterBuiltin = true;
  }
  return _counter;
}

void TransmissionMonitor::setTransmissionCounter(TransmissionCounter* counter, bool shared) {
  if (_counterBuiltin && _counter != NULL) {
    delete _counter;
    _counterBuiltin = false;
  }
  _counter = counter;
  _counterShared = shared;
}
