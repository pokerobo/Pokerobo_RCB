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

#define COORD_LINES_TOTAL               5
#define COORD_LINE_X                    0
#define COORD_LINE_Y                    1
#define COORD_LINE_FLAGS                2
#define COORD_LINE_RAW_X                3
#define COORD_LINE_RAW_Y                4

#define JOYSTICK_PAD_OX                 32
#define JOYSTICK_PAD_OY                 32
#define JOYSTICK_PAD_OR                 30
#define JOYSTICK_PAD_IR                 4
#define JOYSTICK_PAD_PADDING_TOP        1

#define JOYSTICK_PAD_STYLE_CIRCLE       1
#define JOYSTICK_PAD_STYLE_SQUARE1      2
#define JOYSTICK_PAD_STYLE_SQUARE2      3

#define JOYSTICK_PAD_STYLE_ACTIVE       JOYSTICK_PAD_STYLE_SQUARE2

#define idleButtonIcon(offs, buttons, mask, icon) ((offs & mask) ? '-' : (buttons & mask) ? '*' : icon)
#define gte0(x) ((x >= 0) ? x : 0)

char idleButtonIcon_(uint16_t offs, uint16_t buttons, uint16_t mask, char icon) {
  return ((offs & mask) ? '-' : (buttons & mask) ? '*' : icon);
}

DisplayOptions::DisplayOptions(
    lcd_pins_position_t pos=LCD_PINS_ON_BOTTOM) {
  this->_lcdRotation = pos;
}

lcd_pins_position_t DisplayOptions::getLcdRotation() {
  return this->_lcdRotation;
}

DisplayHandler::DisplayHandler(lcd_pins_position_t pos) {
  _options = new DisplayOptions(pos);
  initialize(_options);
}

DisplayHandler::DisplayHandler(DisplayOptions* opts) {
  _options = opts;
  initialize(_options);
}

u8g2_cb_t* convertDisplayRotation(lcd_pins_position_t pos) {
  u8g2_cb_t *rotation = U8G2_R2;
  switch(pos) {
    case LCD_PINS_ON_TOP:
      rotation = U8G2_R0;
      break;
    case LCD_PINS_ON_BOTTOM:
      rotation = U8G2_R2;
      break;
    case LCD_PINS_ON_RIGHT:
      rotation = U8G2_R1;
      break;
    case LCD_PINS_ON_LEFT:
      rotation = U8G2_R3;
      break;
  }
  return rotation;
}

void DisplayHandler::initialize(DisplayOptions* options) {
  u8g2_cb_t *rotation = convertDisplayRotation(options->getLcdRotation());
  _u8g2Ref = new U8G2_ST7567_ENH_DG128064I_1_HW_I2C(rotation,
      LCD_PIN_SCL,
      LCD_PIN_SDA,
      U8X8_PIN_NONE);
}

DisplayOptions* DisplayHandler::getOptions() {
  return _options;
}

int DisplayHandler::begin() {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  Wire.begin();
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(3000, true); // Wire.setWireTimeout(timeout, reset_on_timeout)
  #endif
  _u8g2->setI2CAddress(0x3F * 2);
  // _u8g2->setBusClock(200000);
  _u8g2->setContrast(200);
  _u8g2->begin();

  _u8g2->setFont(u8g2_font_5x8_tf);

  _maxCharHeight = _u8g2->getMaxCharHeight();
  _maxCharWidth = _u8g2->getMaxCharWidth();

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
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->clear();
}

void DisplayHandler::splash(char* title, byte align) {
  if (title == NULL) return;

  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;

  uint8_t left = 0;
  #if !__SPACE_SAVING_MODE__
  if (align == 0) {
    uint8_t pixel = strlen(title) * _maxCharWidth;
    uint16_t displayWidth = _u8g2->getDisplayWidth();
    if (pixel < displayWidth) {
      left = (displayWidth - pixel) / 2;
    }
  }
  #else
  left = align * _maxCharWidth;
  #endif

  uint8_t top = _u8g2->getDisplayHeight() / 2 + _maxCharHeight / 2;

  _u8g2->firstPage();
  do {
    _u8g2->drawStr(left, top, title);
  } while (_u8g2->nextPage());
}

void DisplayHandler::render(ProgramCollection* programCollection) {
  char titleBuffer[20];
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->firstPage();
  do {
    _u8g2->drawButtonUTF8(1, 1 + 1 * (_maxCharHeight), U8G2_BTN_SHADOW0|U8G2_BTN_BW1, 126,  0,  2, " CONTROL PANEL" );
    #if __DEVMODE_DISPLAY_HANDLER__
    _u8g2->drawButtonUTF8(1, 1 + 2 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn1" );
    _u8g2->drawButtonUTF8(1, 1 + 3 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, ">Btn2" );
    _u8g2->drawButtonUTF8(1, 1 + 4 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn3" );
    _u8g2->drawButtonUTF8(1, 1 + 5 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn4" );
    _u8g2->drawButtonUTF8(1, 1 + 6 * (_maxCharHeight + 2), U8G2_BTN_BW1, 126,  0,  1, " Btn5" );
    #else//__DEVMODE_DISPLAY_HANDLER__
    uint8_t total = programCollection->getTotal();
    uint8_t current = programCollection->getCurrentIndex();
    uint8_t focus = programCollection->getFocusIndex();
    uint8_t begin = programCollection->getFrameBegin();
    uint8_t end = programCollection->getFrameEnd();
    for(uint8_t i=begin; i<=end; i++) {
      uint16_t flags = U8G2_BTN_BW1;
      ProgramSticker* capsule = programCollection->getItem(i);
      char* title = capsule->getTitle();
      if (title == NULL) {
        titleBuffer[0] = ' ';
        titleBuffer[1] = 0;
        title = capsule->getTitle(titleBuffer);
      }
      if (i == focus) {
        flags |= U8G2_BTN_INV;
      }
      if (i == current) {
        title[0] = '>';
      } else {
        title[0] = ' ';
      }
      _u8g2->drawButtonUTF8(1, 1 + (i-begin+2) * (_maxCharHeight + 2), flags, 126,  0,  1, title);
    }
    #endif//__DEVMODE_DISPLAY_HANDLER__
  } while (_u8g2->nextPage());
}

void DisplayHandler::render(JoystickAction* message, MessageInterface* commandPacket, TransmissionCounter* counter) {
  if (message == NULL) return;

  int nX = message->getCenterBasedX();
  int nY = message->getCenterBasedY();

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

  uint16_t clickingFlags = message->getTogglingFlags();
  if (clickingFlags & MASK_START_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_START_BUTTON] = '+';
  }
  if (clickingFlags & MASK_SELECT_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_SELECT_BUTTON] = '+';
  }
  if (clickingFlags & MASK_ANALOG_BUTTON) {
    lines[COORD_LINE_FLAGS][POS_ANALOG_BUTTON] = '+';
  }

  uint8_t _statsLx = _maxCharHeight + 1;
  uint8_t _virtualPadLx = _statsLx + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth + 1;
  uint8_t _speedMeterLx = _virtualPadLx + 64;
  uint8_t _counterTy = JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * COORD_LINES_TOTAL + 2;

  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  uint16_t displayHeight = _u8g2->getDisplayHeight();
  _u8g2->firstPage();
  do {
    renderCoordinates_(_statsLx, 0, _maxCharHeight, _maxCharWidth, lines);
    renderJoystickAction_(_virtualPadLx, 0, message);
    renderCommandPacket_(_speedMeterLx, 0, commandPacket);
    renderTransmissionCounter_(_statsLx, _counterTy, _maxCharHeight, _maxCharWidth, counter);
    renderTitle_(_maxCharHeight - 2, displayHeight - 2, source, counter);
  } while (_u8g2->nextPage());
}

void DisplayHandler::renderDirectionState_(char *title, message_source_t source, TransmissionCounter* counter,
    uint8_t &_directionState, uint8_t &_directionTotal) {
  _directionTotal += 1;
  if (_directionTotal >= 10) {
    _directionTotal = 0;
    _directionState += 1;
  }
  if (_directionState >= 5) {
    _directionState = 0;
  }

  uint8_t state = (source == TX_MSG) ? _directionState : (4 - _directionState);
  char arrow = (source == TX_MSG) ? '>' : '<';

  if (source == TX_MSG) {
    if (counter != NULL && counter->continualLossCount > 9) {
      switch (state) {
        case 0:
        case 1:
          title[ 0] = ' ';
          title[ 1] = ' ';
          title[ 2] = ' ';
          title[ 9] = ' ';
          title[10] = ' ';
          title[11] = ' ';
          break;
        case 2:
        case 3:
        case 4:
          title[ 0] = arrow;
          title[ 1] = arrow;
          title[ 2] = ' ';
          title[ 9] = ' ';
          title[10] = arrow;
          title[11] = '|';
          break;
      }
      return;
    }
  }

  switch (state) {
    case 0:
      title[ 0] = arrow;
      title[ 1] = ' ';
      title[ 2] = ' ';
      title[ 9] = ' ';
      title[10] = arrow;
      title[11] = arrow;
      break;
    case 1:
      title[ 0] = arrow;
      title[ 1] = arrow;
      title[ 2] = ' ';
      title[ 9] = ' ';
      title[10] = ' ';
      title[11] = arrow;
      break;
    case 2:
      title[ 0] = ' ';
      title[ 1] = arrow;
      title[ 2] = arrow;
      title[ 9] = ' ';
      title[10] = ' ';
      title[11] = ' ';
      break;
    case 3:
      title[ 0] = ' ';
      title[ 1] = ' ';
      title[ 2] = arrow;
      title[ 9] = arrow;
      title[10] = ' ';
      title[11] = ' ';
      break;
    case 4:
      title[ 0] = ' ';
      title[ 1] = ' ';
      title[ 2] = ' ';
      title[ 9] = arrow;
      title[10] = arrow;
      title[11] = ' ';
      break;
  }
}

void DisplayHandler::renderTitle_(uint8_t lx, uint8_t ty, message_source_t source, TransmissionCounter* counter) {
  #if __SPACE_SAVING_MODE__
  #if __OPTIMIZING_DYNAMIC_MEMORY__
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
  renderDirectionState_(title, source, counter, _directionState, _directionTotal);
  renderTitle_(lx, ty, title);
  #else//__OPTIMIZING_DYNAMIC_MEMORY__
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
  #endif//__OPTIMIZING_DYNAMIC_MEMORY__
  #else//__SPACE_SAVING_MODE__
  if (source == RX_MSG) {
    char title[13] = { '<', '<', ' ', 'T', 'E', 'S', 'T', 'E', 'R', ' ', '<', '<', '\0' };
    renderTitle_(lx, ty, title);
    return;
  }
  char title[13] = { '>', '>', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '>', '>', '\0' };
  renderTitle_(lx, ty, title);
  #endif//__SPACE_SAVING_MODE__
}

void DisplayHandler::renderTitle_(uint8_t lx, uint8_t ty, char* title) {
  if (title == NULL) return;
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->setFontDirection(3);
  _u8g2->drawStr(lx, ty, title);
  _u8g2->setFontDirection(0);
}

void DisplayHandler::renderCoordinates_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, char lines[][JOYSTICK_INFO_COLUMNS]) {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  for (uint8_t i=0; i<COORD_LINES_TOTAL; i++) {
    _u8g2->drawStr(lx, ty + _maxCharHeight + JOYSTICK_PAD_PADDING_TOP + _maxCharHeight * i, lines[i]);
  }
}

void DisplayHandler::drawJoystickCircle(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->drawCircle(Ox, Oy, r, U8G2_DRAW_ALL);
  _u8g2->drawCircle(Ox, Oy, ir, U8G2_DRAW_ALL);

  _u8g2->drawLine(Ox, Oy - ir, Ox, Oy - r - 2);
  _u8g2->drawLine(Ox + ir, Oy, Ox + r + 2, Oy);
  _u8g2->drawLine(Ox, Oy + ir, Ox, Oy + r + 2);
  _u8g2->drawLine(Ox - ir, Oy, Ox - r - 2, Oy);

  renderJoystickPoint_(Ox, Oy, x, y+1);
}

void DisplayHandler::drawJoystickSquare1(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->drawFrame(Ox - r, Oy - r, 2*r, 2*r);
  _u8g2->drawFrame(Ox - ir, Oy - ir, 2*ir, 2*ir);

  _u8g2->drawLine(Ox, Oy - ir, Ox, Oy - r - 2);
  _u8g2->drawLine(Ox + ir, Oy, Ox + r + 2, Oy);
  _u8g2->drawLine(Ox, Oy + ir, Ox, Oy + r + 2);
  _u8g2->drawLine(Ox - ir, Oy, Ox - r - 2, Oy);

  renderJoystickPoint_(Ox, Oy, x, y+1);
}

void DisplayHandler::drawJoystickSquare2(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y) {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->drawFrame(Ox - r, Oy - r, 2*r, 2*r);

  _u8g2->drawLine(Ox - r - 2, Oy - ir, Ox + r + 1, Oy - ir);
  _u8g2->drawLine(Ox - r - 2, Oy + ir, Ox + r + 1, Oy + ir);

  _u8g2->drawLine(Ox - ir - 1, Oy - r - 2, Ox - ir - 1, Oy + r + 2);
  _u8g2->drawLine(Ox + ir - 1, Oy - r - 2, Ox + ir - 1, Oy + r + 2);

  renderJoystickPoint_(Ox, Oy, x, y+1);
}

void DisplayHandler::renderJoystickPoint_(uint8_t Ox, uint8_t Oy, int x, int y) {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  switch (getJoystickPointType()) {
    case LCD_JOYSTICK_POINT_PLUS:
      _u8g2->drawLine(gte0(x+Ox-1), gte0(-y+Oy), gte0(x+Ox+1), gte0(-y+Oy));
      _u8g2->drawLine(gte0(x+Ox), gte0(-y+Oy-1), gte0(x+Ox), gte0(-y+Oy+1));
      break;
    case LCD_JOYSTICK_POINT_SQUARE:
      _u8g2->drawFrame(Ox + x - 1, Oy + (-y) - 1, 3, 3);
      break;
  }
}

void DisplayHandler::renderJoystickAction_(uint8_t lx, uint8_t ty, JoystickAction* action) {
  int x = action->getCenterBasedX();
  int y = action->getCenterBasedY();
  int rX = action->getRadiusOfX();
  int rY = action->getRadiusOfY();
  int pX = map(x, -rX, rX, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR+1);
  int pY = map(y, -rY, rY, -JOYSTICK_PAD_OR, JOYSTICK_PAD_OR+1);
  this->renderJoystickPad_(lx, ty, JOYSTICK_PAD_OR, JOYSTICK_PAD_IR, pX, pY);
}

void DisplayHandler::renderJoystickPad_(uint8_t lx, uint8_t ty, uint8_t r, uint8_t ir, int x, int y) {
  #if JOYSTICK_PAD_STYLE_ACTIVE == JOYSTICK_PAD_STYLE_CIRCLE
  return drawJoystickCircle(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
  #endif
  #if JOYSTICK_PAD_STYLE_ACTIVE == JOYSTICK_PAD_STYLE_SQUARE1
  return drawJoystickSquare1(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
  #endif
  return drawJoystickSquare2(lx + JOYSTICK_PAD_OX, ty + JOYSTICK_PAD_OY, r, ir, x, y);
}

void DisplayHandler::renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket) {
  #if __DEBUG_LOG_DISPLAY_HANDLER__
  Serial.print("DisplayHandler"), Serial.print("::"), Serial.print("renderCommandPacket_"), Serial.print("()"),
      Serial.print(' '), Serial.println("should be overriden");
  #endif
}

void DisplayHandler::renderTransmissionCounter_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, TransmissionCounter* counter) {
  if (counter == NULL) return;

  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->drawLine(lx, ty, lx - 1 + (JOYSTICK_INFO_COLUMNS - 1) * _maxCharWidth, ty);

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
  _u8g2->drawStr(lx, ty + 1 + _maxCharHeight, line);

  sprintf(line, format, counter->packetLossTotal);
  _u8g2->drawStr(lx, ty + 1 + _maxCharHeight * 2, line);
}

void* DisplayHandler::getU8g2Ref() {
  return _u8g2Ref;
}

void DisplayHandler::firstPage() {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  _u8g2->firstPage();
}

uint8_t DisplayHandler::nextPage() {
  U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
  return _u8g2->nextPage();
}

void DisplayHandler::setDisplayRotation(lcd_pins_position_t pos) {
  if (_options != NULL) {
    if (_options->getLcdRotation() != pos) {
      _options->_lcdRotation = pos;
      U8G2 *_u8g2 = (U8G2*)_u8g2Ref;
      _u8g2->setDisplayRotation(convertDisplayRotation(pos));
    }
  }
}

lcd_pins_position_t DisplayHandler::getDisplayRotation() {
  return (_options != NULL) ? _options->getLcdRotation() : LCD_PINS_ON_TOP;
}

lcd_pins_position_t DisplayHandler::nextDisplayRotation(lcd_pins_position_t pos) {
  switch(pos) {
    case LCD_PINS_ON_TOP:
      return LCD_PINS_ON_RIGHT;
    case LCD_PINS_ON_RIGHT:
      return LCD_PINS_ON_BOTTOM;
    case LCD_PINS_ON_BOTTOM:
      return LCD_PINS_ON_LEFT;
    case LCD_PINS_ON_LEFT:
      return LCD_PINS_ON_TOP;
    default:
      return pos;
  }
}

lcd_pins_position_t DisplayHandler::prevDisplayRotation(lcd_pins_position_t pos) {
  switch(pos) {
    case LCD_PINS_ON_TOP:
      return LCD_PINS_ON_LEFT;
    case LCD_PINS_ON_LEFT:
      return LCD_PINS_ON_BOTTOM;
    case LCD_PINS_ON_BOTTOM:
      return LCD_PINS_ON_RIGHT;
    case LCD_PINS_ON_RIGHT:
      return LCD_PINS_ON_TOP;
    default:
      return pos;
  }
}

lcd_joystick_point_t DisplayHandler::getJoystickPointType() {
  return _joystickPointType;
}
