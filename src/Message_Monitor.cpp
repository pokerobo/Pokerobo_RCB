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

void TransmissionCounter::adjust() {
  if (this->ordinalNumber >= 999999UL) {
    this->reset();
  }
}

void TransmissionCounter::reset() {
  this->baselineNumber = 0;
  this->ordinalNumber = 0;
  this->continualLossCount = 0;
  this->packetLossTotal = 0;
}

void TransmissionCounter::beginTransmission() {
  this->ordinalNumber += 1;
  this->packetLossTotal += 1;
}

void TransmissionCounter::confirmTransmissionSuccess() {
  this->continualLossCount = 0;
  this->packetLossTotal -= 1;
}

void TransmissionCounter::confirmTransmissionFailure() {
  this->continualLossCount += 1;
}

void TransmissionCounter::update(uint32_t count) {
  if (this->ordinalNumber == 0) {
    this->baselineNumber = count;
    this->packetLossTotal = 0;
  } else {
    if (count < this->ordinalNumber + 1) {
      this->baselineNumber = count;
      this->packetLossTotal = 0;
    } else if (count == this->ordinalNumber + 1) {
    } else if (count > this->ordinalNumber + 1) {
      this->packetLossTotal += count - this->ordinalNumber - 1;
    }
  }
  this->ordinalNumber = count;
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
