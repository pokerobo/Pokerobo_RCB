#include "Display_Handler.h"

#include <U8g2lib.h>
#include <Wire.h>

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
  u8g2.firstPage();
  do {
    int maxCharHeight = u8g2.getMaxCharHeight();
    for (uint8_t i=0; i<5; i++) {
      u8g2.setCursor(0, maxCharHeight + maxCharHeight * i);
      u8g2.print(lines[i]);
    }
  } while (u8g2.nextPage());
}

bool DisplayHandler::render(JoystickAction* message) {
  char lines[5][8] = {{}, {}, {}, {}, {}};
  sprintf(lines[0], "~X:% 4d", message->getX() - 512);
  sprintf(lines[1], "~Y:% 4d", message->getY() - 512);
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

  return print_(lines);
}
