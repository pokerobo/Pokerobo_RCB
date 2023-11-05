#include "Message_Sender.h"

static const uint8_t JoystickAction::messageSize = 2
    + sizeof(uint16_t)
    + sizeof(uint16_t)
    + sizeof(uint16_t)
    + sizeof(uint32_t);

JoystickAction::JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
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
  return encodeMessage(buf, "JS", _pressingFlags, _x, _y, _extras);
}

SpeedPacket::SpeedPacket(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  _LeftSpeed = leftSpeed;
  _LeftDirection = leftDirection;
  _RightSpeed = rightSpeed;
  _RightDirection = rightDirection;
}

void SpeedPacket::update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  _LeftSpeed = leftSpeed;
  _LeftDirection = leftDirection;
  _RightSpeed = rightSpeed;
  _RightDirection = rightDirection;
}

int SpeedPacket::getLeftSpeed() {
  return _LeftSpeed;
}

byte SpeedPacket::getLeftDirection() {
  return _LeftDirection;
}

int SpeedPacket::getRightSpeed() {
  return _RightSpeed;
}

byte SpeedPacket::getRightDirection() {
  return _RightDirection;
}

void ConsoleMessageRenderer::clear() {}

bool ConsoleMessageRenderer::render(JoystickAction* message) {
  return render(message, NULL);
}

bool ConsoleMessageRenderer::render(JoystickAction* message, SpeedPacket* speedPacket) {
  Serial.print("#"), Serial.print(message->getExtras()), Serial.print(" - ");
  Serial.print("Pressing"), Serial.print("Flags"), Serial.print(": "),
      Serial.print(message->getPressingFlags());
  Serial.print("Clicking"), Serial.print("Flags"), Serial.print(": "),
      Serial.print(message->getClickingFlags());
  Serial.print("; "), Serial.print("X"), Serial.print(": "), Serial.print(message->getX());
  Serial.print("; "), Serial.print("Y"), Serial.print(": "), Serial.print(message->getY());
  Serial.println();
  return true;
}

uint8_t* encodeInteger(uint8_t* store, uint16_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  return store;
}

uint8_t* encodeInteger(uint8_t* store, uint32_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  store[1] = (value >> 16) & 0xff;
  store[1] = (value >> 24) & 0xff;
  return store;
}

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t extras) {
  buf[0] = cmd[0];
  buf[1] = cmd[1];
  encodeInteger(&buf[2], pressed);
  encodeInteger(&buf[4], x);
  encodeInteger(&buf[6], y);
  encodeInteger(&buf[8], extras);
  return buf;
}

uint32_t decodeInteger(uint8_t* arr, int length) {
  uint32_t a0 = arr[0];
  uint32_t a1 = arr[1];
  uint32_t a2 = arr[2];
  uint32_t a3 = arr[3];
  if (length == 2) {
    return (a1 << 8) | a0;
  }
  if (length == 4) {
    return (a3 << 24) | (a2 << 16) | (a1 << 8) | a0;
  }
  return 0;
}

bool decodeMessage(uint8_t* msg, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras) {
  if (msg[0] == cmd[0] && msg[1] == cmd[1]) {
    *buttons = decodeInteger(&msg[2], 2);
    *x = decodeInteger(&msg[4], 2);
    *y = decodeInteger(&msg[6], 2);
    *extras = decodeInteger(&msg[8], 4);
    return true;
  }
  return false;
}
