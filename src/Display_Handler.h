#ifndef __POKEROBO_RCB_DISPLAY_HANDLER_H__
#define __POKEROBO_RCB_DISPLAY_HANDLER_H__

#include "Commons.h"
#include "Message_Exchange.h"
#include "Message_Monitor.h"
#include "Message_Renderer.h"
#include "Program_Collection.h"

#define JOYSTICK_INFO_COLUMNS           8 // 7 chars and '\0'

typedef enum {
  LCD_PINS_ON_TOP = 0,
  LCD_PINS_ON_RIGHT,
  LCD_PINS_ON_BOTTOM,
  LCD_PINS_ON_LEFT
} lcd_pins_position_t;

typedef enum {
  LCD_JOYSTICK_POINT_PLUS,
  LCD_JOYSTICK_POINT_SQUARE
} lcd_joystick_point_t;

class DisplayOptions {
  friend class DisplayHandler;
  public:
    DisplayOptions(lcd_pins_position_t pos=LCD_PINS_ON_BOTTOM);
    lcd_pins_position_t getLcdRotation();
  private:
    lcd_pins_position_t _lcdRotation;
};

class DisplayHandler: public MessageRenderer {
  public:
    DisplayHandler(DisplayOptions* options);
    DisplayHandler(lcd_pins_position_t pos=LCD_PINS_ON_BOTTOM);
    DisplayOptions* getOptions();
    int begin();
    void clear();
    void notify(char* title, byte align = 0);
    void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
    void render(ProgramCollection* programCollection=NULL);
  protected:
    void initialize(DisplayOptions* options);
    virtual void initWire();
    void renderTitle_(uint8_t lx, uint8_t ty, message_source_t source, TransmissionCounter* counter);
    void renderDirectionState_(char *title, message_source_t source, TransmissionCounter* counter, uint8_t &_directionState, uint8_t &_directionTotal);
    void renderTitle_(uint8_t lx, uint8_t ty, char* title);
    void renderCoordinates_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, char lines[][JOYSTICK_INFO_COLUMNS]);
    void renderTransmissionCounter_(uint8_t lx, uint8_t ty, uint8_t _maxCharHeight, uint8_t _maxCharWidth, TransmissionCounter* counter);
    void renderJoystickAction_(uint8_t Ox, uint8_t Oy, JoystickAction* action);
    void renderJoystickPad_(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
    void renderJoystickPoint_(uint8_t Ox, uint8_t Oy, int x, int y);
    virtual void renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket);
    void* getU8g2Ref();
    void firstPage();
    uint8_t nextPage();
    void setDisplayRotation(lcd_pins_position_t pos);
    lcd_pins_position_t getDisplayRotation();
    lcd_pins_position_t nextDisplayRotation(lcd_pins_position_t pos);
    lcd_pins_position_t prevDisplayRotation(lcd_pins_position_t pos);
    virtual lcd_joystick_point_t getJoystickPointType();
  private:
    void drawJoystickCircle(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
    void drawJoystickSquare1(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
    void drawJoystickSquare2(uint8_t Ox, uint8_t Oy, uint8_t r, uint8_t ir, int x, int y);
    DisplayOptions* _options;
    void* _u8g2Ref = NULL;
    uint8_t _maxCharHeight = 8;
    uint8_t _maxCharWidth = 5;
    uint8_t _directionState = 0;
    uint8_t _directionTotal = 0;
    lcd_joystick_point_t _joystickPointType = LCD_JOYSTICK_POINT_SQUARE;
};

#endif
