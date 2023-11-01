#include "Display_Handler.h"

#include <U8g2lib.h>
#include <Wire.h>

#define JOYSTICK_INFO_COLUMNS           8 // 7 chars and '\0'
#define JOYSTICK_PAD_OX                 32
#define JOYSTICK_PAD_OY                 32
#define JOYSTICK_PAD_OR                 30
#define JOYSTICK_PAD_IR                 4

#define JOYSTICK_VISUAL_PAD_CIRCLE      1
#define JOYSTICK_VISUAL_PAD_SQUARE1     2
#define JOYSTICK_VISUAL_PAD_SQUARE2     3

#define JOYSTICK_VISUAL_PAD_STYLE   JOYSTICK_VISUAL_PAD_SQUARE2

#define SPEED_METER_OX                  44 + 64 + 3 + 7
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

U8G2_ST7567_ENH_DG128064I_1_HW_I2C u8g2(U8G2_R2, SCL, SDA, U8X8_PIN_NONE); 

DisplayHandler::DisplayHandler() {
}

int DisplayHandler::begin() {
  Wire.begin();
  Wire.setWireTimeout(3000, true);
  u8g2.setI2CAddress(0x3F * 2); 
  u8g2.setBusClock(200000);
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_12_tf);

  _maxCharHeight = u8g2.getMaxCharHeight();
  _maxCharWidth = u8g2.getMaxCharWidth();
  _virtualPadOx = JOYSTICK_PAD_OX + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth + 2;

#if __RUNNING_LOG_ENABLED__
  // maxCharHeight: 11
  Serial.print("max"), Serial.print("Char"), Serial.print("Height"), Serial.print(": "), Serial.println(_maxCharHeight);
  // maxCharWidth: 6
  Serial.print("max"), Serial.print("Char"), Serial.print("Width"), Serial.print(": "), Serial.println(_maxCharWidth);
#endif

  return 1;
}

bool print_(char lines[][JOYSTICK_INFO_COLUMNS]) {
  int maxCharHeight = u8g2.getMaxCharHeight();
  for (uint8_t i=0; i<5; i++) {
    u8g2.setCursor(0, maxCharHeight + 3 + maxCharHeight * i);
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

  u8g2.drawLine(Ox - r - 2, Oy - ir, Ox + r + 2, Oy - ir);
  u8g2.drawLine(Ox - r - 2, Oy + ir, Ox + r + 2, Oy + ir);

  u8g2.drawLine(Ox - ir, Oy - r - 2, Ox - ir, Oy + r + 2);
  u8g2.drawLine(Ox + ir, Oy - r - 2, Ox + ir, Oy + r + 2);

  u8g2.drawFrame(Ox + x - 1, Oy + (-y) - 1, 3, 3);
}

void drawSpeedPacket(SpeedPacket* speedPacket) {
  int mX = SPEED_METER_OX;
  int mY = SPEED_METER_OY;

  int lw = map(speedPacket->getLeftSpeed(), 0, 256, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t ld = speedPacket->getLeftDirection();
  int rw = map(speedPacket->getRightSpeed(), 0, 256, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t rd = speedPacket->getRightDirection();

  if (ld == 1) {
    u8g2.drawBox(mX - 1 - 7, mY -1 - lw, 7, lw);
  } else if (ld == 2) {
    u8g2.drawBox(mX - 1 - 7, mY + 1, 7, lw);
  }

  if (rd == 1) {
    u8g2.drawBox(mX + 1, mY -1 - rw, 7, rw);
  } else if (rd == 2) {
    u8g2.drawBox(mX + 1, mY + 1, 7, rw);
  }
}

bool DisplayHandler::render(JoystickAction* message) {
  return render(message, NULL);
}

bool DisplayHandler::render(JoystickAction* message, SpeedPacket* speedPacket) {
  int nX = -512 + message->getX();
  int nY = -512 + message->getY();

  char lines[5][JOYSTICK_INFO_COLUMNS] = {{}, {}, {}, {}, {}};
  sprintf(lines[0], "~X:% 4d", nX);
  sprintf(lines[1], "~Y:% 4d", nY);
  sprintf(lines[3], "oX:%4d", message->getOriginX());
  sprintf(lines[4], "oY:%4d", message->getOriginY());

  uint16_t buttons = message->getButtons();
  lines[2][POS_UP_BUTTON] = (buttons & MASK_UP_BUTTON) ? 'U' : '-';
  lines[2][POS_RIGHT_BUTTON] = (buttons & MASK_RIGHT_BUTTON) ? 'R' : '-';
  lines[2][POS_DOWN_BUTTON] = (buttons & MASK_DOWN_BUTTON) ? 'D' : '-';
  lines[2][POS_LEFT_BUTTON] = (buttons & MASK_LEFT_BUTTON) ? 'L' : '-';
  lines[2][POS_START_BUTTON] = (buttons & MASK_START_BUTTON) ? 'S' : '-';
  lines[2][POS_SELECT_BUTTON] = (buttons & MASK_SELECT_BUTTON) ? 'O' : '-';
  lines[2][POS_ANALOG_BUTTON] = (buttons & MASK_ANALOG_BUTTON) ? 'A' : '-';

  uint16_t clickedFlags = message->getClickedFlags();
  if (clickedFlags & MASK_START_BUTTON) {
    lines[2][POS_START_BUTTON] = 's';
  }
  if (clickedFlags & MASK_SELECT_BUTTON) {
    lines[2][POS_SELECT_BUTTON] = 'o';
  }
  if (clickedFlags & MASK_ANALOG_BUTTON) {
    lines[2][POS_ANALOG_BUTTON] = 'a';
  }

  int rX = map(nX, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);
  int rY = map(nY, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);

  u8g2.firstPage();
  do {
    print_(lines);
#if JOYSTICK_VISUAL_PAD_STYLE == JOYSTICK_VISUAL_PAD_SQUARE2
    drawJoystickSquare2(_virtualPadOx, JOYSTICK_PAD_OY, JOYSTICK_PAD_OR, JOYSTICK_PAD_IR, rX, rY);
#endif
    if (speedPacket != NULL) {
      drawSpeedPacket(speedPacket);
    }
  } while (u8g2.nextPage());
}
