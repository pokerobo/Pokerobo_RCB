#include "Display_Handler.h"

#include <U8g2lib.h>
#include <Wire.h>

#define JOYSTICK_PAD_OX 96
#define JOYSTICK_PAD_OY 32
#define JOYSTICK_PAD_OR 30
#define JOYSTICK_PAD_IR 4

#define JOYSTICK_VISUAL_PAD_CIRCLE      1
#define JOYSTICK_VISUAL_PAD_SQUARE1     2
#define JOYSTICK_VISUAL_PAD_SQUARE2     3

#define JOYSTICK_VISUAL_PAD_STYLE   JOYSTICK_VISUAL_PAD_SQUARE2

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
  return 1;
}

bool print_(char lines[][8]) {
  int maxCharHeight = u8g2.getMaxCharHeight();
  for (uint8_t i=0; i<5; i++) {
    u8g2.setCursor(0, maxCharHeight + maxCharHeight * i);
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

bool DisplayHandler::render(JoystickAction* message) {
  int nX = -512 + message->getX();
  int nY = -512 + message->getY();

  char lines[5][8] = {{}, {}, {}, {}, {}};
  sprintf(lines[0], "~X:% 4d", nX);
  sprintf(lines[1], "~Y:% 4d", nY);
  sprintf(lines[3], "oX:%4d", message->getOriginX());
  sprintf(lines[4], "oY:%4d", message->getOriginY());

  uint16_t buttons = message->getButtons();
  lines[2][0] = (buttons & MASK_UP_BUTTON) ? 'U' : '-';
  lines[2][1] = (buttons & MASK_RIGHT_BUTTON) ? 'R' : '-';
  lines[2][2] = (buttons & MASK_DOWN_BUTTON) ? 'D' : '-';
  lines[2][3] = (buttons & MASK_LEFT_BUTTON) ? 'L' : '-';
  lines[2][4] = (buttons & MASK_START_BUTTON) ? 'S' : '-';
  lines[2][5] = (buttons & MASK_SELECT_BUTTON) ? 'O' : '-';
  lines[2][6] = (buttons & MASK_ANALOG_BUTTON) ? 'A' : '-';

  int rX = map(nX, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);
  int rY = map(nY, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);

  u8g2.firstPage();
  do {
    print_(lines);
  #if JOYSTICK_VISUAL_PAD_STYLE == JOYSTICK_VISUAL_PAD_SQUARE2
    drawJoystickSquare2(JOYSTICK_PAD_OX, JOYSTICK_PAD_OY, JOYSTICK_PAD_OR, JOYSTICK_PAD_IR, rX, rY);
#endif
  } while (u8g2.nextPage());
}
