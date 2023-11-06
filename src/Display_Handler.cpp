#include "Display_Handler.h"

#include <U8g2lib.h>
#include <Wire.h>

#define COORD_LINES_TOTAL               5
#define COORD_LINE_X                    0
#define COORD_LINE_Y                    1
#define COORD_LINE_CLICKING_FLAGS       2
#define COORD_LINE_RAW_X                3
#define COORD_LINE_RAW_Y                4

#define JOYSTICK_INFO_COLUMNS           8 // 7 chars and '\0'
#define JOYSTICK_PAD_OX                 32
#define JOYSTICK_PAD_OY                 32
#define JOYSTICK_PAD_OR                 30
#define JOYSTICK_PAD_IR                 4
#define JOYSTICK_PAD_PADDING_TOP        1

#define JOYSTICK_VISUAL_PAD_CIRCLE      1
#define JOYSTICK_VISUAL_PAD_SQUARE1     2
#define JOYSTICK_VISUAL_PAD_SQUARE2     3

#define JOYSTICK_VISUAL_PAD_STYLE   JOYSTICK_VISUAL_PAD_SQUARE2

#define SPEED_METER_OX                  3 + 7
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

#define idleButtonIcon(buttons, mask, icon) ((JOYSTICK_DISABLED_BUTTONS & mask) ? '-' : (buttons & mask) ? '*' : icon)

U8G2_ST7567_ENH_DG128064I_1_HW_I2C u8g2(U8G2_R2, SCL, SDA, U8X8_PIN_NONE); 

DisplayHandler::DisplayHandler() {
}

int DisplayHandler::begin() {
  Wire.begin();
  Wire.setWireTimeout(3000, true);
  u8g2.setI2CAddress(0x3F * 2); 
  u8g2.setBusClock(200000);
  u8g2.begin();
  u8g2.setFont(u8g2_font_5x8_tf);

  _maxCharHeight = u8g2.getMaxCharHeight();
  _maxCharWidth = u8g2.getMaxCharWidth();

#if __RUNNING_LOG_ENABLED__
  // maxCharHeight: 8
  Serial.print("max"), Serial.print("Char"), Serial.print("Height"), Serial.print(": "), Serial.println(_maxCharHeight);
  // maxCharWidth: 5
  Serial.print("max"), Serial.print("Char"), Serial.print("Width"), Serial.print(": "), Serial.println(_maxCharWidth);
#endif

  return 1;
}

void DisplayHandler::clear() {
  u8g2.clear();
}

void DisplayHandler::splash(char* title) {
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 32 + _maxCharHeight / 2, title);
  } while (u8g2.nextPage());
}

void renderTitle_(uint8_t lx, uint8_t ty, char* title);
bool renderCoordinates_(uint8_t lx, uint8_t ty, char lines[][JOYSTICK_INFO_COLUMNS], uint8_t _maxCharHeight);
void renderJoystickPad_(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
void renderSpeedWeight_(uint8_t lx, uint8_t ty, SpeedPacket* speedPacket);
void renderTransmissionCounter_(uint8_t lx, uint8_t ty, TransmissionCounter* counter, uint8_t _maxCharHeight, uint8_t _maxCharWidth);

bool DisplayHandler::render(JoystickAction* message) {
  return render(message, NULL);
}

bool DisplayHandler::render(JoystickAction* message, SpeedPacket* speedPacket, TransmissionCounter* counter) {
  int nX = -512 + message->getX();
  int nY = -512 + message->getY();

  char title[13] = { '>', '>', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '>', '>', '\0' };

  if (message->getSource() == RX_MSG) {
    title[ 0] = '<';
    title[ 1] = '<';
    title[ 2] = ' ';
    title[ 3] = 'T';
    title[ 4] = 'E';
    title[ 5] = 'S';
    title[ 6] = 'T';
    title[ 7] = 'E';
    title[ 8] = 'R';
    title[ 9] = ' ';
    title[10] = '<';
    title[11] = '<';
  }

  char lines[COORD_LINES_TOTAL][JOYSTICK_INFO_COLUMNS] = { {}, {}, {}, {}, {} };

  sprintf(lines[COORD_LINE_X], "~X:% 4d", nX);
  sprintf(lines[COORD_LINE_Y], "~Y:% 4d", nY);
  sprintf(lines[COORD_LINE_RAW_X], "oX:%4d", message->getOriginX());
  sprintf(lines[COORD_LINE_RAW_Y], "oY:%4d", message->getOriginY());

  uint16_t buttons = message->getPressingFlags();
  lines[COORD_LINE_CLICKING_FLAGS][POS_UP_BUTTON] = idleButtonIcon(buttons, MASK_UP_BUTTON, 'U');
  lines[COORD_LINE_CLICKING_FLAGS][POS_RIGHT_BUTTON] = idleButtonIcon(buttons, MASK_RIGHT_BUTTON, 'R');
  lines[COORD_LINE_CLICKING_FLAGS][POS_DOWN_BUTTON] = idleButtonIcon(buttons, MASK_DOWN_BUTTON, 'D');
  lines[COORD_LINE_CLICKING_FLAGS][POS_LEFT_BUTTON] = idleButtonIcon(buttons, MASK_LEFT_BUTTON, 'L');
  lines[COORD_LINE_CLICKING_FLAGS][POS_START_BUTTON] = idleButtonIcon(buttons, MASK_START_BUTTON, 'S');
  lines[COORD_LINE_CLICKING_FLAGS][POS_SELECT_BUTTON] = idleButtonIcon(buttons, MASK_SELECT_BUTTON, 'O');
  lines[COORD_LINE_CLICKING_FLAGS][POS_ANALOG_BUTTON] = idleButtonIcon(buttons, MASK_ANALOG_BUTTON, 'A');

  uint16_t clickingFlags = message->getClickingFlags();
  if (clickingFlags & MASK_START_BUTTON) {
    lines[COORD_LINE_CLICKING_FLAGS][POS_START_BUTTON] = '+';
  }
  if (clickingFlags & MASK_SELECT_BUTTON) {
    lines[COORD_LINE_CLICKING_FLAGS][POS_SELECT_BUTTON] = '+';
  }
  if (clickingFlags & MASK_ANALOG_BUTTON) {
    lines[COORD_LINE_CLICKING_FLAGS][POS_ANALOG_BUTTON] = '+';
  }

  int rX = map(nX, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);
  int rY = map(nY, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);

  uint8_t _statsLx = _maxCharHeight + 1;
  uint8_t _virtualPadOx = _statsLx + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth + 1;
  uint8_t _speedMeterLx = _virtualPadOx + 64;
  uint8_t _counterTy = JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * COORD_LINES_TOTAL + 2;

  u8g2.firstPage();
  do {
    renderCoordinates_(_statsLx, 0, lines, _maxCharHeight);
    renderJoystickPad_(_virtualPadOx, 0, JOYSTICK_PAD_OR, JOYSTICK_PAD_IR, rX, rY);
    renderSpeedWeight_(_speedMeterLx, 0, speedPacket);
    renderTransmissionCounter_(_statsLx, _counterTy, counter, _maxCharHeight, _maxCharWidth);
    renderTitle_(_maxCharHeight - 2, 62, title);
  } while (u8g2.nextPage());
}

void renderTitle_(uint8_t lx, uint8_t ty, char* title) {
  u8g2.setFontDirection(3);
  u8g2.drawStr(lx, ty, title);
  u8g2.setFontDirection(0);
}

bool renderCoordinates_(uint8_t lx, uint8_t ty, char lines[][JOYSTICK_INFO_COLUMNS], uint8_t _maxCharHeight) {
  for (uint8_t i=0; i<COORD_LINES_TOTAL; i++) {
    u8g2.setCursor(lx, ty + _maxCharHeight + JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * i);
    u8g2.print(lines[i]);
  }
}

void drawJoystickCircle(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  u8g2.drawCircle(Ox, Oy, r, U8G2_DRAW_ALL);
  u8g2.drawCircle(Ox, Oy, ir, U8G2_DRAW_ALL);

  u8g2.drawLine(Ox, Oy - ir, Ox, Oy - r - 2);
  u8g2.drawLine(Ox + ir, Oy, Ox + r + 2, Oy);
  u8g2.drawLine(Ox, Oy + ir, Ox, Oy + r + 2);
  u8g2.drawLine(Ox - ir, Oy, Ox - r - 2, Oy);

  u8g2.drawFrame(Ox + x - 1, Oy + (-y) - 1, 3, 3);
}

void drawJoystickSquare1(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  u8g2.drawFrame(Ox - r, Oy - r, 2*r, 2*r);
  u8g2.drawFrame(Ox - ir, Oy - ir, 2*ir, 2*ir);

  u8g2.drawLine(Ox, Oy - ir, Ox, Oy - r - 2);
  u8g2.drawLine(Ox + ir, Oy, Ox + r + 2, Oy);
  u8g2.drawLine(Ox, Oy + ir, Ox, Oy + r + 2);
  u8g2.drawLine(Ox - ir, Oy, Ox - r - 2, Oy);

  u8g2.drawFrame(Ox + x - 1, Oy + (-y) - 1, 3, 3);
}

void drawJoystickSquare2(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  u8g2.drawFrame(Ox - r, Oy - r, 2*r, 2*r);

  u8g2.drawLine(Ox - r - 2, Oy - ir, Ox + r + 1, Oy - ir);
  u8g2.drawLine(Ox - r - 2, Oy + ir, Ox + r + 1, Oy + ir);

  u8g2.drawLine(Ox - ir - 1, Oy - r - 2, Ox - ir - 1, Oy + r + 2);
  u8g2.drawLine(Ox + ir - 1, Oy - r - 2, Ox + ir - 1, Oy + r + 2);

  u8g2.drawFrame(Ox + x - 1, Oy + (-y) - 1, 3, 3);
}

void renderJoystickPad_(uint8_t lx, uint8_t ty, uint8_t r, uint8_t ir, int x, int y) {
  uint8_t Ox = lx + JOYSTICK_PAD_OX;
  uint8_t Oy = ty + JOYSTICK_PAD_OY;
#if JOYSTICK_VISUAL_PAD_STYLE == JOYSTICK_VISUAL_PAD_CIRCLE
  return drawJoystickCircle(Ox, Oy, r, ir, x, y);
#endif
#if JOYSTICK_VISUAL_PAD_STYLE == JOYSTICK_VISUAL_PAD_SQUARE1
  return drawJoystickSquare1(Ox, Oy, r, ir, x, y);
#endif
  return drawJoystickSquare2(Ox, Oy, r, ir, x, y);
}

void renderSpeedWeight_(uint8_t lx, uint8_t ty, SpeedPacket* speedPacket) {
  if (speedPacket == NULL) return;

  int mX = lx + SPEED_METER_OX;
  int mY = ty + SPEED_METER_OY;

  int lw = map(speedPacket->getLeftSpeed(), 0, 256, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t ld = speedPacket->getLeftDirection();
  int rw = map(speedPacket->getRightSpeed(), 0, 256, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t rd = speedPacket->getRightDirection();

  u8g2.drawHLine(mX - 2 - 7, mY, (7 + 2)*2);

  if (ld == 1) {
    u8g2.drawBox(mX - 1 - 7, mY - lw, 7, lw);
  } else if (ld == 2) {
    u8g2.drawBox(mX - 1 - 7, mY + 1, 7, lw);
  }

  if (rd == 1) {
    u8g2.drawBox(mX + 1, mY - rw, 7, rw);
  } else if (rd == 2) {
    u8g2.drawBox(mX + 1, mY + 1, 7, rw);
  }
}

void renderTransmissionCounter_(uint8_t lx, uint8_t ty, TransmissionCounter* counter, uint8_t _maxCharHeight, uint8_t _maxCharWidth) {
  if (counter == NULL) return;

  u8g2.drawHLine(lx, ty, (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth);

  char line[8] = {};

  sprintf(line, "% 7ld", counter->sendingTotal);
  u8g2.setCursor(lx, ty + 1 + _maxCharHeight);
  u8g2.print(line);

  sprintf(line, "% 7ld", counter->packetLossTotal);
  u8g2.setCursor(lx, ty + 1 + _maxCharHeight * 2);
  u8g2.print(line);
}
