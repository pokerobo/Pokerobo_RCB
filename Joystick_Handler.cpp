#include "Joystick_Handler.h"

int buttons[] = {
  PIN_UP_BUTTON,
  PIN_RIGHT_BUTTON,
  PIN_DOWN_BUTTON,
  PIN_LEFT_BUTTON,
  PIN_START_BUTTON,
  PIN_SELECT_BUTTON,
  PIN_ANALOG_BUTTON
};

JoystickHandler::JoystickHandler(MessageSender* messageSender) {
  _messageSender = messageSender;
}

int JoystickHandler::begin() {
  for(int i; i <7 ; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);  
  }
}

int JoystickHandler::loop() {
  _count += 1;

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(PIN_JOYSTICK_X_AXIS);
  uint16_t y = analogRead(PIN_JOYSTICK_Y_AXIS);

#if __RUNNING_LOG_ENABLED__
    char log[32] = "";
    sprintf(log, "%d,%d,%d", pressed, x, y);
    Serial.print("M1"), Serial.print(": "), Serial.println(log);
#endif

  x = map(x, 0, JOYSTICK_MAX_X, 0, 1024);
  y = map(y, 0, JOYSTICK_MAX_Y, 0, 1024);

#if __RUNNING_LOG_ENABLED__
    sprintf(log, "%d,%d,%d", pressed, x, y);
    Serial.print("M2"), Serial.print(": "), Serial.println(log);
#endif

  if (isChanged(x, y, pressed)) {
    uint8_t msg[12] = {};
    encodeMessage(msg, "JS", pressed, x, y, _count);
    if (_messageSender != NULL) {
      bool ok = _messageSender->write(msg, sizeof(msg));
      if (ok) {
#if __RUNNING_LOG_ENABLED__
        Serial.print("->"), Serial.println("OK");
#endif
      } else {
#if __RUNNING_LOG_ENABLED__
        Serial.print("->"), Serial.println("FAIL");
#endif
      }
    }
    return 2;
  } else {
    return 1;
  }
}

bool JoystickHandler::isChanged(int16_t x, int16_t y, uint32_t buttons) {
  return !(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || buttons;
}

#if !__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t buttonStates = 0;

  for (int i = 0; i < 7; i++) {
    buttonStates |= ((digitalRead(buttons[i]) == LOW) ? 1 : 0) << i;
  }

  return buttonStates;
}

#else //__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t pressed = 0;

  if(digitalRead(PIN_UP_BUTTON)==LOW) {
    pressed |= BIT_UP_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("UP"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_RIGHT_BUTTON)==LOW) {
    pressed |= BIT_RIGHT_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("RIGHT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_DOWN_BUTTON)==LOW) {
    pressed |= BIT_DOWN_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("DOWN"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_LEFT_BUTTON)==LOW) {
    pressed |= BIT_LEFT_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("LEFT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_START_BUTTON)==LOW) {
    pressed |= BIT_START_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("START"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_SELECT_BUTTON)==LOW) {
    pressed |= BIT_SELECT_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("SELECT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_ANALOG_BUTTON)==LOW) {
    pressed |= BIT_ANALOG_BUTTON;
#if __RUNNING_LOG_ENABLED__
    Serial.print("ANALOG"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  return pressed;
}
#endif //__JOYSTICK_READ_BUTTONS_DEBUG__