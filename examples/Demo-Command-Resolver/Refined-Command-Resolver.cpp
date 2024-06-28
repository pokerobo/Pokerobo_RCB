#include "Refined-Command-Resolver.h"

CommandPacket* RefinedCommandResolver::resolve(CommandPacket* commandPacket, JoystickAction* action) {
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
  int _coeff = getCoefficient();

  if (y > MOVING_COMMAND_BOUND_Y) {
    ld = rd = 1;
    if (x < -MOVING_COMMAND_BOUND_X) {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * _coeff / 10;
      enaVal = int_abs(y) - (r - dx);
      enbVal = int_abs(y) - dx;
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      enaVal = enbVal = int_abs(y);
    } else {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * _coeff / 10;
      enaVal = int_abs(y) - dx;
      enbVal = int_abs(y) - (r - dx);
    }
  } else if (y <= MOVING_COMMAND_BOUND_Y && y >= -MOVING_COMMAND_BOUND_Y) {
    if (x < -MOVING_COMMAND_BOUND_X) {
      ld = 2;
      rd = 1;
      enaVal = int_abs(x);
      enbVal = int_abs(x);
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      // do nothing
    } else {
      ld = 1;
      rd = 2;
      enaVal = int_abs(x);
      enbVal = int_abs(x);
    }
  } else {
    ld = rd = 2;
    if (x < -MOVING_COMMAND_BOUND_X) {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * _coeff / 10;
      enaVal = int_abs(y) - (r - dx);
      enbVal = int_abs(y) - dx;
    } else if (x >= -MOVING_COMMAND_BOUND_X && x <= MOVING_COMMAND_BOUND_X) {
      enaVal = enbVal = int_abs(y);
    } else {
      int r = int_min(int_abs(x), int_abs(y));
      int dx = r * _coeff / 10;
      enaVal = int_abs(y) - dx;
      enbVal = int_abs(y) - (r - dx);
    }
  }

  enaVal = int_max(enaVal, 0);
  enbVal = int_max(enbVal, 0);

  enaVal = map(enaVal, 0, 512, 0, MOVING_COMMAND_WEIGHT_MAX);
  enbVal = map(enbVal, 0, 512, 0, MOVING_COMMAND_WEIGHT_MAX);

  command->update(enaVal, ld, enbVal, rd, isReversed());

  return command;
}
