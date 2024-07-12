#include "Message_Monitor.h"

uint32_t TransmissionCounter::getBaselineNumber() {
  return baselineNumber;
}

uint32_t TransmissionCounter::getOrdinalNumber() {
  return ordinalNumber;
}

uint32_t TransmissionCounter::getContinualLossCount() {
  return continualLossCount;
}

uint32_t TransmissionCounter::getPacketLossTotal() {
  return packetLossTotal;
}

bool TransmissionMonitor::isCounterBuiltin() {
  return _counterBuiltin;
}

bool TransmissionMonitor::isCounterShared() {
  return _counterShared;
}

TransmissionCounter* TransmissionMonitor::getTransmissionCounter(bool createIfNotFound) {
  if (_counter == NULL && createIfNotFound) {
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
