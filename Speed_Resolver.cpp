#include "Speed_Resolver.h"

#define BOUND_X    40
#define BOUND_Y    40

SpeedPacket* SpeedResolver::resolve(SpeedPacket* packet, JoystickAction* action, int coeff=1, bool rotatable=false) {
  if (packet == NULL) {
    return packet;
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
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = abs(y);
    } else {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - dx;
      enbVal = abs(y) - (r - dx);
    }
  } else if (y <= BOUND_Y && y >= -BOUND_Y) {
    // do nothing
  } else {
    ld = rd = 2;
    if (x < -BOUND_X) {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = abs(y);
    } else {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - dx;
      enbVal = abs(y) - (r - dx);
    }
  }

  enaVal = max(enaVal, 0);
  enbVal = max(enbVal, 0);

  enaVal = map(enaVal, 0, 512, 0, 256);
  enbVal = map(enbVal, 0, 512, 0, 256);

  packet->update(enaVal, ld, enbVal, rd);

  return packet;
}