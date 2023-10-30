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

char lines[2][24] = {{}, {}};

int DisplayHandler::check() {
  u8g2.firstPage();
  do {
      u8g2.setCursor(0, 23);
      u8g2.print(lines[0]);
      u8g2.setCursor(0, 23 + u8g2.getMaxCharHeight() + 1);
      u8g2.print(lines[1]);
  } while (u8g2.nextPage());
}

bool DisplayHandler::render(JoystickAction* message) {
  sprintf(lines[0], "Fix X:% 4d - Y:% 4d", message->getX() - 512, message->getY() - 512);
  sprintf(lines[1], "Raw X:%4d - Y:%4d", message->getOriginX(), message->getOriginY());
  check();
  return true;
}
