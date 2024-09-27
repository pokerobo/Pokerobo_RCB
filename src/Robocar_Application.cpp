#include "Robocar_Application.h"
#include <U8g2lib.h>

//-------------------------------------------------------------------------------------------------

MovingCommandPacket::MovingCommandPacket(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  update(leftSpeed, leftDirection, rightSpeed, rightDirection);
}

void MovingCommandPacket::update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  _LeftSpeed = leftSpeed;
  _LeftDirection = leftDirection;
  _RightSpeed = rightSpeed;
  _RightDirection = rightDirection;
}

void MovingCommandPacket::update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection, bool reversed) {
  update(leftSpeed, leftDirection, rightSpeed, rightDirection);
  _reversed = reversed;
}

int MovingCommandPacket::getLeftSpeed() {
  return _LeftSpeed;
}

byte MovingCommandPacket::getLeftDirection() {
  return _LeftDirection;
}

int MovingCommandPacket::getRightSpeed() {
  return _RightSpeed;
}

byte MovingCommandPacket::getRightDirection() {
  return _RightDirection;
}

bool MovingCommandPacket::isReversed() {
  return _reversed;
}

const uint8_t MovingCommandPacket::messageSize = sizeof(uint8_t) +
    sizeof(uint8_t) +
    sizeof(uint8_t);

uint8_t MovingCommandPacket::length() {
  return messageSize;
}

uint8_t* MovingCommandPacket::serialize(uint8_t* buf, uint8_t len) {
  if (len < messageSize) {
    return NULL;
  }

  uint8_t directionFlags = 0;

  switch(_LeftDirection) {
    case 1:
      directionFlags |= 0b0001;
      break;
    case 2:
      directionFlags |= 0b0010;
      break;
  }

  switch(_RightDirection) {
    case 1:
      directionFlags |= 0b0100;
      break;
    case 2:
      directionFlags |= 0b1000;
      break;
  }

  if (_reversed) {
    directionFlags |= 0b10000000;
  }

  buf[0] = directionFlags;
  buf[1] = _LeftSpeed;
  buf[2] = _RightSpeed;

  return buf;
}

CommandPacket* MovingCommandPacket::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }

  uint8_t directionFlags = buf[0];
  if (directionFlags & 0b0001) {
    _LeftDirection = 1;
  }
  if (directionFlags & 0b0010) {
    _LeftDirection = 2;
  }
  if (directionFlags & 0b0100) {
    _RightDirection = 1;
  }
  if (directionFlags & 0b1000) {
    _RightDirection = 2;
  }
  if (directionFlags & 0b10000000) {
    _reversed = true;
  }

  _LeftSpeed = buf[1];
  _RightSpeed = buf[2];

  return this;
}

//-------------------------------------------------------------------------------------------------

MovingCommandResolver::MovingCommandResolver(bool reversed) {
  _reversed = reversed;
}

bool MovingCommandResolver::isReversed() {
  return _reversed;
}

void MovingCommandResolver::setReversed(bool reversed) {
  _reversed = reversed;
}

int MovingCommandResolver::getCoefficient() {
  return _coefficient;
}

void MovingCommandResolver::setCoefficient(int coefficient) {
  _coefficient = coefficient;
}

CommandPacket* MovingCommandResolver::create() {
  return new MovingCommandPacket();
}

CommandPacket* MovingCommandResolver::resolve(CommandPacket* commandPacket, JoystickAction* action) {
  MovingCommandPacket* command = (MovingCommandPacket*) commandPacket;

  if (command == NULL) {
    return command;
  }

  int x = -512 + action->getX();
  int y = -512 + action->getY();
  uint8_t ld = 0;
  uint8_t rd = 0;
  int enaVal = 0;
  int enbVal = 0;

  if (y > MOVING_COMMAND_BOUND_Y) {
    ld = rd = 1;
    if (x < -MOVING_COMMAND_BOUND_X) {
      onForwardLeft(x, y, enaVal, ld, enbVal, rd);
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      onForward(x, y, enaVal, ld, enbVal, rd);
    } else {
      onForwardRight(x, y, enaVal, ld, enbVal, rd);
    }
  } else if (y <= MOVING_COMMAND_BOUND_Y && y >= -MOVING_COMMAND_BOUND_Y) {
    if (x < -MOVING_COMMAND_BOUND_X) {
      onLeft(x, y, enaVal, ld, enbVal, rd);
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      // do nothing
    } else {
      onRight(x, y, enaVal, ld, enbVal, rd);
    }
  } else {
    ld = rd = 2;
    if (x < -MOVING_COMMAND_BOUND_X) {
      onBackwardLeft(x, y, enaVal, ld, enbVal, rd);
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      onBackward(x, y, enaVal, ld, enbVal, rd);
    } else {
      onBackwardRight(x, y, enaVal, ld, enbVal, rd);
    }
  }

  enaVal = int_max(enaVal, 0);
  enbVal = int_max(enbVal, 0);

  enaVal = map(enaVal, 0, 512, 0, MOVING_COMMAND_WEIGHT_MAX);
  enbVal = map(enbVal, 0, 512, 0, MOVING_COMMAND_WEIGHT_MAX);

  #if __DEBUG_LOG_COMMAND_RESOLVER__
  Serial.print("enA"), Serial.print(':'), Serial.print(' '), Serial.print(enaVal);
  Serial.print(' '), Serial.print('-'), Serial.print(' ');
  Serial.print("enB"), Serial.print(':'), Serial.print(' '), Serial.print(enbVal);
  Serial.println();
  #endif

  command->update(enaVal, ld, enbVal, rd, isReversed());

  return command;
}

void MovingCommandResolver::release(CommandPacket* command) {
  if (command == NULL) return;
  MovingCommandPacket* movingCommand = (MovingCommandPacket*) command;
  delete movingCommand;
}

void MovingCommandResolver::onForwardLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  int r = int_min(int_abs(x), int_abs(y));
  int dx = r * getCoefficient() / 10;
  leftSpeed = int_abs(y) - (r - dx);
  rightSpeed = int_abs(y) - dx;
}

void MovingCommandResolver::onForward(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  leftSpeed = rightSpeed = int_abs(y);
}

void MovingCommandResolver::onForwardRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  int r = int_min(int_abs(x), int_abs(y));
  int dx = r * getCoefficient() / 10;
  leftSpeed = int_abs(y) - dx;
  rightSpeed = int_abs(y) - (r - dx);
}

void MovingCommandResolver::onLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  // should be implemented
}

void MovingCommandResolver::onRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  // should be implemented
}

void MovingCommandResolver::onBackwardRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  int r = int_min(int_abs(x), int_abs(y));
  int dx = r * getCoefficient() / 10;
  leftSpeed = int_abs(y) - dx;
  rightSpeed = int_abs(y) - (r - dx);
}

void MovingCommandResolver::onBackwardLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  int r = int_min(int_abs(x), int_abs(y));
  int dx = r * getCoefficient() / 10;
  leftSpeed = int_abs(y) - (r - dx);
  rightSpeed = int_abs(y) - dx;
}

void MovingCommandResolver::onBackward(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection) {
  leftSpeed = rightSpeed = int_abs(y);
}

//-------------------------------------------------------------------------------------------------

const uint8_t CtrlCarMessageSerializer::messageSize = MasterContext::messageSize +
    JoystickAction::messageSize +
    MovingCommandPacket::messageSize;

uint8_t CtrlCarMessageSerializer::getSize() {
  return messageSize;
}

int CtrlCarMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  uint8_t offset = 0;

  MasterContext contextPacket;
  if (contextPacket.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += contextPacket.length();
  MasterContext* context = &contextPacket;

  JoystickAction controlAction;
  if (controlAction.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += controlAction.length();
  JoystickAction* action = &controlAction;

  #if __DEBUG_LOG_MESSAGE_SERIALIZER__
  char log[32] = { 0 };
  buildJoystickActionLogStr(log, action->getPressingFlags(), action->getX(), action->getY(), action->getExtras());
  Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
      Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
  Serial.println();
  #endif

  MovingCommandPacket commandPacket;
  MessageInterface* command = commandPacket.deserialize(msg + offset);

  if (processor != NULL) {
    return processor->process(context, action, command);
  }

  return 0;
}

//-------------------------------------------------------------------------------------------------

const uint8_t MovingMessageSerializer::messageSize = MESSAGE_SIGNATURE_SIZE +
    JoystickAction::messageSize +
    MovingCommandPacket::messageSize;

uint8_t MovingMessageSerializer::getSize() {
  return messageSize;
}

int MovingMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  if (msg[0] == 'J' && msg[1] == 'S') {
    JoystickAction controlAction;
    if (controlAction.deserialize(msg + MESSAGE_SIGNATURE_SIZE) == NULL) {
      return -1;
    }
    JoystickAction* action = &controlAction;

    #if __DEBUG_LOG_MESSAGE_SERIALIZER__
    char log[32] = { 0 };
    buildJoystickActionLogStr(log, action->getPressingFlags(), action->getX(), action->getY(), action->getExtras());
    Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
        Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
    Serial.println();
    #endif

    MovingCommandPacket movingCommandInstance;
    MessageInterface* commandPacket = movingCommandInstance.deserialize(
        msg + MESSAGE_SIGNATURE_SIZE + JoystickAction::messageSize);

    if (processor != NULL) {
      return processor->process(NULL, action, commandPacket);
    }

    return 0;
  }

  #if __LEGACY_MESSAGE_SERIALIZER__
  uint8_t offset = 0;

  MasterContext contextPacket;
  if (contextPacket.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += contextPacket.length();
  MasterContext* context = &contextPacket;

  JoystickAction controlAction;
  if (controlAction.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += controlAction.length();
  JoystickAction* action = &controlAction;

  #if __DEBUG_LOG_MESSAGE_SERIALIZER__
  char log[32] = { 0 };
  buildJoystickActionLogStr(log, action->getPressingFlags(), action->getX(), action->getY(), action->getExtras());
  Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
      Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
  Serial.println();
  #endif

  MovingCommandPacket commandPacket;
  MessageInterface* command = commandPacket.deserialize(msg + offset);

  if (processor != NULL) {
    return processor->process(context, action, command);
  }

  return 0;
  #else
  return CtrlCarMessageSerializer::decode(msg, processor);
  #endif
}

//-------------------------------------------------------------------------------------------------

#define SPEED_METER_WIDTH               7
#define SPEED_METER_OX                  3 + SPEED_METER_WIDTH
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

void MovingDisplayHandler::renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket) {
  if (commandPacket == NULL) return;

  MovingCommandPacket* movingCommand = (MovingCommandPacket*) commandPacket;

  U8G2* u8g2 = (U8G2*)getU8g2Ref();

  int mX = lx + SPEED_METER_OX;
  int mY = ty + SPEED_METER_OY;

  int lw = map(movingCommand->getLeftSpeed(), 0, MOVING_COMMAND_WEIGHT_MAX, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t ld = movingCommand->getLeftDirection();
  int rw = map(movingCommand->getRightSpeed(), 0, MOVING_COMMAND_WEIGHT_MAX, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t rd = movingCommand->getRightDirection();

  u8g2->drawLine(mX - 2 - SPEED_METER_WIDTH, mY, mX + 1 + SPEED_METER_WIDTH, mY);

  switch(ld) {
    case 1:
      u8g2->drawBox(mX - 1 - SPEED_METER_WIDTH, mY - lw, SPEED_METER_WIDTH, lw);
      break;
    case 2:
      u8g2->drawBox(mX - 1 - SPEED_METER_WIDTH, mY + 1, SPEED_METER_WIDTH, lw);
      break;
  }

  switch(rd) {
    case 1:
      u8g2->drawBox(mX + 1, mY - rw, SPEED_METER_WIDTH, rw);
      break;
    case 2:
      u8g2->drawBox(mX + 1, mY + 1, SPEED_METER_WIDTH, rw);
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void MovingSerialConsole::clear() {}

void MovingSerialConsole::notify(char* title, byte align) {}

void MovingSerialConsole::render(JoystickAction* message, MessageInterface* commandPacket, TransmissionCounter* counter) {
  MovingCommandPacket* movingCommand = (MovingCommandPacket*) commandPacket;
  Serial.print('#'), Serial.print(message->getExtras()), Serial.print(' '), Serial.print('-'), Serial.print(' ');
  Serial.print("Pressing"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getPressingFlags());
  Serial.print(';'), Serial.print(' ');
  Serial.print("Toggling"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getTogglingFlags());
  Serial.print(';'), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.print(message->getX());
  Serial.print(';'), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.print(message->getY());
  Serial.println();
}
