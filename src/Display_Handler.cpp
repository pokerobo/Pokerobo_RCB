#include "Display_Handler.h"

#include <U8g2lib.h>
#include <Wire.h>

#ifndef __DEBUG_LOG_DISPLAY_HANDLER__
#define __DEBUG_LOG_DISPLAY_HANDLER__   __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_DISPLAY_HANDLER__

#ifndef __DEVMODE_DISPLAY_HANDLER__
#define __DEVMODE_DISPLAY_HANDLER__     0
#endif//__DEVMODE_DISPLAY_HANDLER__

#ifndef LCD_PIN_SCL
#define LCD_PIN_SCL                     SCL
#endif//LCD_PIN_SCL

#ifndef LCD_PIN_SDA
#define LCD_PIN_SDA                     SDA
#endif//LCD_PIN_SDA

#define SCREEN_HEIGHT                   64
#define SCREEN_WIDTH                   128

#define COORD_LINES_TOTAL               5
#define COORD_LINE_X                    0
#define COORD_LINE_Y                    1
#define COORD_LINE_FLAGS                2
#define COORD_LINE_RAW_X                3
#define COORD_LINE_RAW_Y                4

#define JOYSTICK_INFO_COLUMNS           8 // 7 chars and '\0'
#define JOYSTICK_PAD_OX                 32
#define JOYSTICK_PAD_OY                 32
#define JOYSTICK_PAD_OR                 30
#define JOYSTICK_PAD_IR                 4
#define JOYSTICK_PAD_PADDING_TOP        1

#define JOYSTICK_PAD_STYLE_CIRCLE       1
#define JOYSTICK_PAD_STYLE_SQUARE1      2
#define JOYSTICK_PAD_STYLE_SQUARE2      3

#define JOYSTICK_PAD_STYLE_ACTIVE       JOYSTICK_PAD_STYLE_SQUARE2

#define SPEED_METER_WIDTH               7
#define SPEED_METER_OX                  3 + SPEED_METER_WIDTH
#define SPEED_METER_OY                  32
#define SPEED_METER_MAX_HEIGHT          30

#define idleButtonIcon(offs, buttons, mask, icon) ((offs & mask) ? '-' : (buttons & mask) ? '*' : icon)

char idleButtonIcon_(uint16_t offs, uint16_t buttons, uint16_t mask, char icon) {
  return ((offs & mask) ? '-' : (buttons & mask) ? '*' : icon);
}

U8G2_ST7567_ENH_DG128064I_1_HW_I2C u8g2(U8G2_R2, LCD_PIN_SCL, LCD_PIN_SDA, U8X8_PIN_NONE);

int DisplayHandler::begin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  // Wire.setWireTimeout(timeout, reset_on_timeout)
  Wire.setWireTimeout(3000, true);
#endif
  u8g2.setI2CAddress(0x3F * 2); 
  u8g2.setBusClock(200000);
  u8g2.begin();

  u8g2.setFont(u8g2_font_5x8_tf);

  _maxCharHeight = u8g2.getMaxCharHeight();
  _maxCharWidth = u8g2.getMaxCharWidth();

#if __DEBUG_LOG_DISPLAY_HANDLER__
  // maxCharHeight: 8
  Serial.print("max"), Serial.print("Char"), Serial.print("Height"), Serial.print(':'),
      Serial.print(' '), Serial.println(_maxCharHeight);
  // maxCharWidth: 5
  Serial.print("max"), Serial.print("Char"), Serial.print("Width"), Serial.print(':'),
      Serial.print(' '), Serial.println(_maxCharWidth);
#endif

  return 1;
}

void DisplayHandler::clear() {
  u8g2.clear();
}

void DisplayHandler::splash(char* title, byte align) {
  if (title == NULL) return;

  uint8_t left = 0;
#if !__SPACE_SAVING_MODE__
  uint8_t len = strlen(title);
  uint8_t pixel = len * _maxCharWidth;
  if (pixel < SCREEN_WIDTH) {
    if (align == 0) {
      left = (SCREEN_WIDTH - pixel) / 2;
    }
  }
#else
  left = align * _maxCharWidth;
#endif

  u8g2.firstPage();
  do {
    u8g2.drawStr(left, SCREEN_HEIGHT / 2 + _maxCharHeight / 2, title);
  } while (u8g2.nextPage());
}

void DisplayHandler::render(ProgramCollection* programCollection) {
  u8g2.firstPage();
  do {
    u8g2.drawButtonUTF8(1, 1 + 1 * (_maxCharHeight), U8G2_BTN_SHADOW0|U8G2_BTN_BW1, 126,  0,  2, "CONTROL PANEL" );
#if __DEVMODE_DISPLAY_HANDLER__
    u8g2.drawButtonUTF8(1, 1 + 2 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn1" );
    u8g2.drawButtonUTF8(1, 1 + 3 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, ">Btn2" );
    u8g2.drawButtonUTF8(1, 1 + 4 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn3" );
    u8g2.drawButtonUTF8(1, 1 + 5 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn4" );
    u8g2.drawButtonUTF8(1, 1 + 6 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn5" );
#else
    uint8_t total = programCollection->getTotal();
    for(uint8_t i=0; i<total; i++) {
      ProgramCapsule* capsule = programCollection->getItem(i);
      u8g2.drawButtonUTF8(1, 1 + (i+2) * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, capsule->getTitle());
    }
#endif
  } while (u8g2.nextPage());
}

void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source);
void renderTitle_(uint8_t lx, uint8_t ty, char* title);
void renderCoordinates_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, char lines[][JOYSTICK_INFO_COLUMNS]);
void renderJoystickPad_(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
void renderSpeedWeight_(uint8_t lx, uint8_t ty, MovingCommand* movingCommand);
void renderTransmissionCounter_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, TransmissionCounter* counter);

void DisplayHandler::render(JoystickAction* message, MovingCommand* movingCommand, TransmissionCounter* counter) {
  if (message == NULL) return;

  int nX = -512 + message->getX();
  int nY = -512 + message->getY();

  message_source_t source = message->getSource();

  uint16_t buttonOffs = (source == RX_MSG) ? 0 : JOYSTICK_DISABLED_BUTTONS;

  char lines[COORD_LINES_TOTAL][JOYSTICK_INFO_COLUMNS] = { {}, {}, {}, {}, {} };

#if __OPTIMIZING_DYNAMIC_MEMORY__
  char fmt[JOYSTICK_INFO_COLUMNS] = { 0 };
  fmt[0] = '~';
  fmt[1] = 'X';
  fmt[2] = ':';
  fmt[3] = '%';
  fmt[4] = ' ';
  fmt[5] = '4';
  fmt[6] = 'd';
  fmt[7] = '\0';

  sprintf(lines[COORD_LINE_X], fmt, nX);
  fmt[1] = 'Y';
  sprintf(lines[COORD_LINE_Y], fmt, nY);

  fmt[0] = 'o';
  fmt[1] = 'X';
  fmt[2] = ':';
  fmt[3] = '%';
  fmt[4] = '4';
  fmt[5] = 'd';
  fmt[6] = '\0';

  sprintf(lines[COORD_LINE_RAW_X], fmt, message->getOriginX());
  fmt[1] = 'Y';
  sprintf(lines[COORD_LINE_RAW_Y], fmt, message->getOriginY());
#else
  char fmt1[JOYSTICK_INFO_COLUMNS] = { '~', 'X', ':', '%', ' ', '4', 'd', '\0' };
  sprintf(lines[COORD_LINE_X], fmt1, nX);
  fmt1[1] = 'Y';
  sprintf(lines[COORD_LINE_Y], fmt1, nY);

  char fmt2[JOYSTICK_INFO_COLUMNS - 1] = { 'o', 'X', ':', '%', '4', 'd', '\0' };
  sprintf(lines[COORD_LINE_RAW_X], fmt2, message->getOriginX());
  fmt2[1] = 'Y';
  sprintf(lines[COORD_LINE_RAW_Y], fmt2, message->getOriginY());
#endif

  uint16_t pressingFlags = message->getPressingFlags();
  lines[COORD_LINE_FLAGS][POS_UP_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_UP_BUTTON, 'U');
  lines[COORD_LINE_FLAGS][POS_RIGHT_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_RIGHT_BUTTON, 'R');
  lines[COORD_LINE_FLAGS][POS_DOWN_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_DOWN_BUTTON, 'D');
  lines[COORD_LINE_FLAGS][POS_LEFT_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_LEFT_BUTTON, 'L');
  lines[COORD_LINE_FLAGS][POS_START_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_START_BUTTON, 'S');
  lines[COORD_LINE_FLAGS][POS_SELECT_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_SELECT_BUTTON, 'O');
  lines[COORD_LINE_FLAGS][POS_ANALOG_BUTTON] = idleButtonIcon(buttonOffs, pressingFlags, MASK_ANALOG_BUTTON, 'A');

  uint16_t clickingFlags = message->getClickingFlags();
  if (clickingFlags & MASK_START_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_START_BUTTON] = '+';
  }
  if (clickingFlags & MASK_SELECT_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_SELECT_BUTTON] = '+';
  }
  if (clickingFlags & MASK_ANALOG_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_ANALOG_BUTTON] = '+';
  }

  int rX = map(nX, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);
  int rY = map(nY, -512, 512, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR);

  uint8_t _statsLx = _maxCharHeight + 1;
  uint8_t _virtualPadLx = _statsLx + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth + 1;
  uint8_t _speedMeterLx = _virtualPadLx + 64;
  uint8_t _counterTy = JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * COORD_LINES_TOTAL + 2;

  u8g2.firstPage();
  do {
    renderCoordinates_(_statsLx, 0, _maxCharHeight, _maxCharWidth, lines);
    renderJoystickPad_(_virtualPadLx, 0, JOYSTICK_PAD_OR, JOYSTICK_PAD_IR, rX, rY);
    renderSpeedWeight_(_speedMeterLx, 0, movingCommand);
    renderTransmissionCounter_(_statsLx, _counterTy, _maxCharHeight, _maxCharWidth, counter);
    renderTitle_(_maxCharHeight - 2, SCREEN_HEIGHT - 2, source);
  } while (u8g2.nextPage());
}

#if __SPACE_SAVING_MODE__
#if __OPTIMIZING_DYNAMIC_MEMORY__
void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source) {
  char title[13] = { 0 };
  if (source == TX_MSG) {
    title[ 0] = '>';
    title[ 1] = '>';
    title[ 2] = ' ';
    title[ 3] = 'P';
    title[ 4] = 'L';
    title[ 5] = 'A';
    title[ 6] = 'Y';
    title[ 7] = 'E';
    title[ 8] = 'R';
    title[ 9] = ' ';
    title[10] = '>';
    title[11] = '>';
  }
  if (source == RX_MSG) {
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
  renderTitle_(lx, ty, title);
}
#else//__OPTIMIZING_DYNAMIC_MEMORY__
void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source) {
  char title[13] = { '>', '>', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '>', '>', '\0' };
  if (source == RX_MSG) {
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
  renderTitle_(lx, ty, title);
}
#endif//__OPTIMIZING_DYNAMIC_MEMORY__
#else//__SPACE_SAVING_MODE__
void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source) {
  if (source == RX_MSG) {
    char title[13] = { '<', '<', ' ', 'T', 'E', 'S', 'T', 'E', 'R', ' ', '<', '<', '\0' };
    renderTitle_(lx, ty, title);
    return;
  }
  char title[13] = { '>', '>', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '>', '>', '\0' };
  renderTitle_(lx, ty, title);
}
#endif//__SPACE_SAVING_MODE__

void renderTitle_(uint8_t lx, uint8_t ty, char* title) {
  if (title == NULL) return;
  u8g2.setFontDirection(3);
  u8g2.drawStr(lx, ty, title);
  u8g2.setFontDirection(0);
}

void renderCoordinates_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, char lines[][JOYSTICK_INFO_COLUMNS]) {
  for (uint8_t i=0; i<COORD_LINES_TOTAL; i++) {
    u8g2.drawStr(lx, ty + _maxCharHeight + JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * i, lines[i]);
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
#if JOYSTICK_PAD_STYLE_ACTIVE == JOYSTICK_PAD_STYLE_CIRCLE
  return drawJoystickCircle(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
#endif
#if JOYSTICK_PAD_STYLE_ACTIVE == JOYSTICK_PAD_STYLE_SQUARE1
  return drawJoystickSquare1(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
#endif
  return drawJoystickSquare2(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
}

void renderSpeedWeight_(uint8_t lx, uint8_t ty, MovingCommand* movingCommand) {
  if (movingCommand == NULL) return;

  int mX = lx + SPEED_METER_OX;
  int mY = ty + SPEED_METER_OY;

  int lw = map(movingCommand->getLeftSpeed(), 0, MOVING_COMMAND_WEIGHT_MAX, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t ld = movingCommand->getLeftDirection();
  int rw = map(movingCommand->getRightSpeed(), 0, MOVING_COMMAND_WEIGHT_MAX, 0, SPEED_METER_MAX_HEIGHT);
  uint8_t rd = movingCommand->getRightDirection();

  u8g2.drawLine(mX - 2 - SPEED_METER_WIDTH, mY, mX + 1 + SPEED_METER_WIDTH, mY);

  switch(ld) {
    case 1:
      u8g2.drawBox(mX - 1 - SPEED_METER_WIDTH, mY - lw, SPEED_METER_WIDTH, lw);
      break;
    case 2:
      u8g2.drawBox(mX - 1 - SPEED_METER_WIDTH, mY + 1, SPEED_METER_WIDTH, lw);
      break;
  }

  switch(rd) {
    case 1:
      u8g2.drawBox(mX + 1, mY - rw, SPEED_METER_WIDTH, rw);
      break;
    case 2:
      u8g2.drawBox(mX + 1, mY + 1, SPEED_METER_WIDTH, rw);
      break;
  }
}

void renderTransmissionCounter_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, TransmissionCounter* counter) {
  if (counter == NULL) return;

  u8g2.drawLine(lx, ty, lx - 1 + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth, ty);

  char line[JOYSTICK_INFO_COLUMNS] = {};

#if __OPTIMIZING_DYNAMIC_MEMORY__
  char format[6] = { 0 };
  format[0] = '%';
  format[1] = ' ';
  format[2] = '7';
  format[3] = 'l';
  format[4] = 'd';
  format[5] = '\0';
#else
  char format[6] = { '%', ' ', '7', 'l', 'd', '\0' };
#endif

  sprintf(line, format, counter->ordinalNumber - counter->baselineNumber);
  u8g2.drawStr(lx, ty + 1 + _maxCharHeight, line);

  sprintf(line, format, counter->packetLossTotal);
  u8g2.drawStr(lx, ty + 1 + _maxCharHeight * 2, line);
}
