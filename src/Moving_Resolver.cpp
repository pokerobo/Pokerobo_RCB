#include <U8g2lib.h>
#include "Moving_Resolver.h"

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

  _LeftSpeed = buf[1];
  _RightSpeed = buf[2];

  return this;
}

//-------------------------------------------------------------------------------------------------

#define BOUND_X    40
#define BOUND_Y    40

CommandPacket* MovingCommandResolver::create() {
  return new MovingCommandPacket();
}

CommandPacket* MovingCommandResolver::resolve(CommandPacket* commandPacket, JoystickAction* action, int coeff, bool rotatable) {
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

  if (y > BOUND_Y) {
    ld = rd = 1;
    if (x < -BOUND_X) {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * coeff / 10;
      enaVal = int_abs(y) - (r - dx);
      enbVal = int_abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = int_abs(y);
    } else {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * coeff / 10;
      enaVal = int_abs(y) - dx;
      enbVal = int_abs(y) - (r - dx);
    }
  } else if (y <= BOUND_Y && y >= -BOUND_Y) {
    // do nothing
  } else {
    ld = rd = 2;
    if (x < -BOUND_X) {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * coeff / 10;
      enaVal = int_abs(y) - (r - dx);
      enbVal = int_abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = int_abs(y);
    } else {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * coeff / 10;
      enaVal = int_abs(y) - dx;
      enbVal = int_abs(y) - (r - dx);
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

  command->update(enaVal, ld, enbVal, rd);

  return command;
}

void MovingCommandResolver::release(CommandPacket* command) {
  if (command == NULL) return;
  MovingCommandPacket* movingCommand = (MovingCommandPacket*) command;
  delete movingCommand;
}

//-------------------------------------------------------------------------------------------------

const uint8_t MovingMessageSerializer::messageSize = strlen(MESSAGE_SIGNATURE) +
    JoystickAction::messageSize +
    MovingCommandPacket::messageSize;

uint8_t MovingMessageSerializer::getSize() {
  return messageSize;
}

int MovingMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  if (msg[0] == 'J' && msg[1] == 'S') {
    JoystickAction controlAction;
    if (controlAction.deserialize(msg + strlen(MESSAGE_SIGNATURE)) == NULL) {
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
        msg + strlen(MESSAGE_SIGNATURE) + JoystickAction::messageSize);

    if (processor != NULL) {
      return processor->process(NULL, action, commandPacket);
    }

    return 0;
  }
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

#define SPEED_METER_WIDTH               7
#define SPEED_METER_OX                  3 + SPEED_METER_WIDTH
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

void MovingDisplayHandler::renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket) {
  if (commandPacket == NULL) return;

  MovingCommandPacket* movingCommand = (MovingCommandPacket*) commandPacket;

  U8G2* u8g2 = (U8G2*) _u8g2Ref;

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

void MovingSerialConsole::splash(char* title, byte align) {}

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
