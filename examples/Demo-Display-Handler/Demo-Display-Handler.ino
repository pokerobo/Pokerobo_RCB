#include <Pokerobo_RCB.h>

class SimpleDisplayHandler: public DisplayHandler {
  public:
    void render(JoystickAction* message) {
      this->firstPage();
      do {
        this->renderJoystickAction_(0, 0, message);
      } while (this->nextPage());
    }
  protected:
    lcd_joystick_point_t getJoystickPointType() {
      return LCD_JOYSTICK_POINT_PLUS;
    }
};

SimpleDisplayHandler displayHandler;
JoystickHandler joystickHandler;

int counter = 0;

void setup() {
  Serial.begin(57600);
  displayHandler.begin();
  joystickHandler.begin();
}

void loop() {
  if (counter < 1) {
    counter++;
    displayHandler.splash("Joystick Screen Demo");
    delay(2000);
  }
  JoystickAction joystickAction;
  displayHandler.render(joystickHandler.input(&joystickAction));
  delay(50);
}
