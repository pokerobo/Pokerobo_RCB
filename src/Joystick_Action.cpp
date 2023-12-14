#include "Joystick_Action.h"

const uint8_t JoystickAction::messageSize = sizeof(uint16_t)
    + sizeof(uint16_t)
    + sizeof(uint16_t)
    + sizeof(uint32_t);

JoystickAction::JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  update(buttons, x, y, extras);
}

void JoystickAction::update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  _pressingFlags = buttons;
  _x = x;
  _y = y;
  _extras = extras;
}

void JoystickAction::setOrigin(uint16_t x, uint16_t y) {
  _originX = x;
  _originY = y;
}

void JoystickAction::setClickingFlags(uint16_t clickingFlags) {
  _clickingTrail = clickingFlags;
}

void JoystickAction::setSource(message_source_t source) {
  _source = source;
}

message_source_t JoystickAction::getSource() {
  return _source;
}

uint16_t JoystickAction::getPressingFlags() {
  return _pressingFlags;
}

uint16_t JoystickAction::getClickingFlags() {
  return _clickingTrail;
}

uint16_t JoystickAction::getX() {
  return _x;
}

uint16_t JoystickAction::getY() {
  return _y;
}

uint16_t JoystickAction::getOriginX() {
  return _originX;
}

uint16_t JoystickAction::getOriginY() {
  return _originY;
}

uint32_t JoystickAction::getExtras() {
  return _extras;
}

uint8_t JoystickAction::length() {
  return messageSize;
}

uint8_t* JoystickAction::serialize(uint8_t* buf, uint8_t len) {
  if (len < messageSize) {
    return NULL;
  }
  return encodeMessage(buf, NULL, _pressingFlags, _x, _y, _extras);
}

MessageInterface* JoystickAction::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }

  decodeMessage(buf, NULL, &(_pressingFlags), &(_x), &(_y), &(_extras));

  return this;
}

char* buildJoystickActionLogStr(char* log, uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  #if __OPTIMIZING_DYNAMIC_MEMORY__
  char fmt[12] = { 0 };
  fmt[ 0] = '%';
  fmt[ 1] = 'd';
  fmt[ 2] = ',';
  fmt[ 3] = '%';
  fmt[ 4] = 'd';
  fmt[ 5] = ',';
  fmt[ 6] = '%';
  fmt[ 7] = 'd';
  fmt[ 8] = ',';
  fmt[ 9] = '%';
  fmt[10] = 'd';
  fmt[11] = '\0';
  #else
  char fmt[12] = { '%', 'd', ',', '%', 'd', ',', '%', 'd', ',', '%', 'd', '\0' };
  #endif

  sprintf(log, fmt, buttons, x, y, extras);

  return log;
}
