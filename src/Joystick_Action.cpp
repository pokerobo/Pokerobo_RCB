#include "Joystick_Action.h"

#if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V1
const uint8_t JoystickAction::messageSize = 0 // SIGNATURE
    + sizeof(uint16_t) // pressingFlags
    + sizeof(uint16_t) // Joystick-X
    + sizeof(uint16_t) // Joystick-Y
    + sizeof(uint32_t);
#else
const uint8_t JoystickAction::messageSize = 0 // SIGNATURE
    + sizeof(uint16_t) // pressingFlags
    + sizeof(uint16_t) // togglingFlags
    + sizeof(uint16_t) // Joystick-X
    + sizeof(uint16_t) // Joystick-Y
    + sizeof(uint32_t);
#endif

int16_t JoystickAction::getRadiusOfX() {
  return 512;
}

int16_t JoystickAction::getRadiusOfY() {
  return 512;
}

JoystickAction::JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  update(buttons, x, y, extras);
}

void JoystickAction::update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  _pressingFlags = buttons;
  _x = x;
  _y = y;
  _extras = extras;
}

JoystickAction::JoystickAction(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras) {
  update(x, y, pressingFlags, togglingFlags, extras);
}

void JoystickAction::update(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras) {
  _x = x;
  _y = y;
  _pressingFlags = pressingFlags;
  _togglingFlags = togglingFlags;
  _extras = extras;
}

void JoystickAction::setOrigin(uint16_t x, uint16_t y) {
  _originX = x;
  _originY = y;
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

uint16_t JoystickAction::getTogglingFlags() {
  return _togglingFlags;
}

bool JoystickAction::isButtonClicked(uint16_t button) {
  return (_togglingFlags & button) == button;
}

bool JoystickAction::isButtonPressed(uint16_t button) {
  return (_pressingFlags & button) == button;
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

int16_t JoystickAction::getCenterBasedX() {
  return -getRadiusOfX() + _x;
}

int16_t JoystickAction::getCenterBasedY() {
  return -getRadiusOfY() + _y;
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
  #if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V1
  return encodeMessage(buf, NULL, _pressingFlags, _x, _y, _extras);
  #else
  return encodeMessage(buf, NULL, _x, _y, _pressingFlags, _togglingFlags, _extras);
  #endif
}

MessageInterface* JoystickAction::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }
  #if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V1
  decodeMessage(buf, NULL, &(_pressingFlags), &(_x), &(_y), &(_extras));
  #else
  decodeMessage(buf, NULL, &(_x), &(_y), &(_pressingFlags), &(_togglingFlags), &(_extras));
  #endif

  return this;
}

#if JOYSTICK_CHECKING_CHANGE
bool JoystickAction::isChanged() {
  int16_t x = this->getX();
  int16_t y = this->getY();
  uint32_t buttons = this->getPressingFlags();
  return !(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || buttons;
}
#endif

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
