#include <U8g2lib.h>
#include "Moving_Resolver.h"

#define BOUND_X    40
#define BOUND_Y    40

#define SPEED_METER_WIDTH               7
#define SPEED_METER_OX                  3 + SPEED_METER_WIDTH
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

MovingCommand* MovingResolver::resolve(MovingCommand* command, JoystickAction* action, int coeff, bool rotatable) {
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

  command->update(enaVal, ld, enbVal, rd);

  return command;
}

//-------------------------------------------------------------------------------------------------

const uint8_t MovingMessageSerializer::messageSize = strlen(MESSAGE_SIGNATURE) +
    JoystickAction::messageSize +
    MovingCommand::messageSize;

uint8_t MovingMessageSerializer::getSize() {
  return messageSize;
}

int MovingMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  uint16_t buttons;
  uint16_t jX, jY;
  uint32_t count;

  bool ok = decodeMessage(msg, MESSAGE_SIGNATURE, &buttons, &jX, &jY, &count);

  if (!ok) {
    return -1;
  }

  #if __DEBUG_LOG_RF24_TRANCEIVER__
  char log[32] = { 0 };
  buildJoystickActionLogStr(log, buttons, jX, jY, count);
  Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
      Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
  Serial.println();
  #endif

  JoystickAction message(buttons, jX, jY, count);

  MovingCommand movingCommandInstance;
  MessageInterface* commandPacket = movingCommandInstance.deserialize(
      msg + strlen(MESSAGE_SIGNATURE) + JoystickAction::messageSize);

  if (processor != NULL) {
    return processor->process(&message, commandPacket);
  }

  return 0;
}

//-------------------------------------------------------------------------------------------------

void MovingDisplayHandler::renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket) {
  if (commandPacket == NULL) return;

  MovingCommand* movingCommand = (MovingCommand*) commandPacket;

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

void ConsoleMessageRenderer::clear() {}

void ConsoleMessageRenderer::splash(char* title, byte align) {}

void ConsoleMessageRenderer::render(JoystickAction* message, MessageInterface* commandPacket, TransmissionCounter* counter) {
  MovingCommand* movingCommand = (MovingCommand*) commandPacket;
  Serial.print('#'), Serial.print(message->getExtras()), Serial.print(' '), Serial.print('-'), Serial.print(' ');
  Serial.print("Pressing"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getPressingFlags());
  Serial.print(';'), Serial.print(' ');
  Serial.print("Clicking"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getClickingFlags());
  Serial.print(';'), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.print(message->getX());
  Serial.print(';'), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.print(message->getY());
  Serial.println();
}
