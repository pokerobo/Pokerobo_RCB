#include "Program_Collections.h"

ProgramTransmitter::ProgramTransmitter(JoystickHandler* joystickHandler,
    RF24Tranceiver* tranceiver, uint64_t address) {
  if (address != 0) {
    _rf24Address = address;
  }
  _rf24Tranceiver = tranceiver;
  _joystickHandler = joystickHandler;
}

int ProgramTransmitter::begin() {
  return _rf24Tranceiver->begin(RF24_TX, _rf24Address);
}

int ProgramTransmitter::check(void* action) {
  return _joystickHandler->check((JoystickAction*) action);
}

int ProgramTransmitter::close() {
  _rf24Tranceiver->reset(RF24_TX);
  return 0;
}

ProgramReceiver::ProgramReceiver(RF24Tranceiver* tranceiver, uint64_t address) {
  if (address != 0) {
    _rf24Address = address;
  }
  _rf24Tranceiver = tranceiver;
  }

int ProgramReceiver::begin() {
  return _rf24Tranceiver->begin(RF24_RX, _rf24Address);
}

int ProgramReceiver::check(void* action) {
  return _rf24Tranceiver->check();
}

int ProgramReceiver::close() {
  _rf24Tranceiver->reset(RF24_RX);
  return 0;
}
