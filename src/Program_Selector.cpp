#include "Program_Selector.h"

#ifndef __DEBUG_LOG_PROGRAM_SELECTOR__
#define __DEBUG_LOG_PROGRAM_SELECTOR__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_PROGRAM_SELECTOR__

int ProgramSelector::begin(uint8_t mode) {
  _mode = mode;
  return 0;
}

int ProgramSelector::check() {
  int result = move_();
  switch(_currentState) {
    case PROGRAM_NRF24_REAL_TRANSMITTER:
    case PROGRAM_NRF24_TEST_TRANSMITTER:
      delay(50);
      break;
    case PROGRAM_NRF24_REAL_RECEIVER:
    case PROGRAM_NRF24_TEST_RECEIVER:
      delay(10);
      break;
  }
  return result;
}

int ProgramSelector::move_() {
  JoystickAction message;
  _joystickHandler->input(&message);
  uint16_t clickingFlags = message.getClickingFlags();
  if (clickingFlags & PROGRAM_MENU_TOGGLE_BUTTON) {
    switch(_currentState) {
      case PROGRAM_NRF24_REAL_TRANSMITTER:
        _rf24Tranceiver->reset(RF24_TX);
        if (_mode == PROGRAM_MODE_PLAYER) {
          _rf24Tranceiver->begin(RF24_TX);
          _currentState = PROGRAM_NRF24_TEST_TRANSMITTER;
#if __DEBUG_LOG_PROGRAM_SELECTOR__
          Serial.print("ProgramSelector"), Serial.print("::"), Serial.print("move_"), Serial.print(": "),
            Serial.print("PROGRAM_NRF24"), Serial.print("_REAL"), Serial.print("_TRANSMITTER"),
            Serial.print('-'), Serial.print('>'),
            Serial.print("PROGRAM_NRF24"), Serial.print("_TEST"), Serial.print("_TRANSMITTER"),
            Serial.println();
#endif
        }
        if (_mode == PROGRAM_MODE_TESTER) {
          _rf24Tranceiver->begin(RF24_RX);
          _currentState = PROGRAM_NRF24_TEST_RECEIVER;
#if __DEBUG_LOG_PROGRAM_SELECTOR__
          Serial.print("ProgramSelector"), Serial.print("::"), Serial.print("move_"), Serial.print(": "),
            Serial.print("PROGRAM_NRF24"), Serial.print("_REAL"), Serial.print("_TRANSMITTER"),
            Serial.print('-'), Serial.print('>'),
            Serial.print("PROGRAM_NRF24"), Serial.print("_TEST"), Serial.print("_RECEIVER"),
            Serial.println();
#endif
        }
        break;
      case PROGRAM_NRF24_TEST_TRANSMITTER:
        _rf24Tranceiver->reset(RF24_TX);
        _rf24Tranceiver->begin(RF24_TX, _rf24Address);
        _currentState = PROGRAM_NRF24_REAL_TRANSMITTER;
#if __DEBUG_LOG_PROGRAM_SELECTOR__
        Serial.print("ProgramSelector"), Serial.print("::"), Serial.print("move_"), Serial.print(": "),
          Serial.print("PROGRAM_NRF24"), Serial.print("_TEST"), Serial.print("_TRANSMITTER"),
          Serial.print('-'), Serial.print('>'),
          Serial.print("PROGRAM_NRF24"), Serial.print("_REAL"), Serial.print("_TRANSMITTER"),
          Serial.println();
#endif
        break;
      case PROGRAM_NRF24_TEST_RECEIVER:
        _rf24Tranceiver->reset(RF24_RX);
        _rf24Tranceiver->begin(RF24_TX, _rf24Address);
        _currentState = PROGRAM_NRF24_REAL_TRANSMITTER;
#if __DEBUG_LOG_PROGRAM_SELECTOR__
        Serial.print("ProgramSelector"), Serial.print("::"), Serial.print("move_"), Serial.print(": "),
          Serial.print("PROGRAM_NRF24"), Serial.print("_TEST"), Serial.print("_RECEIVER"),
          Serial.print('-'), Serial.print('>'),
          Serial.print("PROGRAM_NRF24"), Serial.print("_REAL"), Serial.print("_TRANSMITTER"),
          Serial.println();
#endif
        break;
    }
  } else {
    switch(_currentState) {
      case PROGRAM_NRF24_REAL_TRANSMITTER:
      case PROGRAM_NRF24_TEST_TRANSMITTER:
        return _joystickHandler->check(&message);
      case PROGRAM_NRF24_REAL_RECEIVER:
      case PROGRAM_NRF24_TEST_RECEIVER:
        return _rf24Tranceiver->check();
    }
  }
  return 0;
}

void ProgramSelector::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

void ProgramSelector::set(JoystickHandler* joystickHandler) {
  _joystickHandler = joystickHandler;
}

void ProgramSelector::set(RF24Tranceiver* tranceiver, uint64_t address) {
  _rf24Tranceiver = tranceiver;
  _rf24Address = address;
}
