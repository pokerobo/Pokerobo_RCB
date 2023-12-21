#include "Joystick_Handler.h"

#ifndef __DEBUG_LOG_JOYSTICK_HANDLER__
#define __DEBUG_LOG_JOYSTICK_HANDLER__ __RUNNING_LOG_ENABLED__
#endif//__DEBUG_LOG_JOYSTICK_HANDLER__

#ifndef JOYSTICK_DETECTION_TOTAL
#define JOYSTICK_DETECTION_TOTAL  20
#endif//JOYSTICK_DETECTION_TOTAL

#ifndef JOYSTICK_DETECTION_DELAY
#define JOYSTICK_DETECTION_DELAY  50
#endif//JOYSTICK_DETECTION_DELAY

#ifndef JOYSTICK_HIGH_LEVEL_PINS
#define JOYSTICK_HIGH_LEVEL_PINS  0b1111111
#endif//JOYSTICK_HIGH_LEVEL_PINS

#if __JOYSTICK_FUNDUINO_SHIELD__
#define BUTTON_PRESS_PIN_VALUES   0b0000000
#else
#define BUTTON_PRESS_PIN_VALUES   0b0111111
#endif

#define CLICK_STYLE_BUTTONS       MASK_START_BUTTON | MASK_SELECT_BUTTON | MASK_ANALOG_BUTTON

const int JoystickHandler::pinOfButtons[] = {
  PIN_UP_BUTTON,
  PIN_RIGHT_BUTTON,
  PIN_DOWN_BUTTON,
  PIN_LEFT_BUTTON,
  PIN_START_BUTTON,
  PIN_SELECT_BUTTON,
  PIN_ANALOG_BUTTON
};

#if __STRICT_MODE__
void JoystickHandler::verify() {
  pinOfButtons[BIT_UP_BUTTON] = PIN_UP_BUTTON;
  pinOfButtons[BIT_RIGHT_BUTTON] = PIN_RIGHT_BUTTON;
  pinOfButtons[BIT_DOWN_BUTTON] = PIN_DOWN_BUTTON;
  pinOfButtons[BIT_LEFT_BUTTON] = PIN_LEFT_BUTTON;
  pinOfButtons[BIT_START_BUTTON] = PIN_START_BUTTON;
  pinOfButtons[BIT_SELECT_BUTTON] = PIN_SELECT_BUTTON;
  pinOfButtons[BIT_ANALOG_BUTTON] = PIN_ANALOG_BUTTON;
}
#endif

void JoystickHandler::detect() {
  uint16_t middleX[JOYSTICK_DETECTION_TOTAL] = {};
  uint16_t middleY[JOYSTICK_DETECTION_TOTAL] = {};
  uint16_t minX = 1024;
  uint16_t minY = 1024;

  for(uint8_t i=0; i<JOYSTICK_DETECTION_TOTAL; i++) {
    delay(JOYSTICK_DETECTION_DELAY);
    middleX[i] = analogRead(JOYSTICK_PIN_X_AXIS);
    middleY[i] = analogRead(JOYSTICK_PIN_Y_AXIS);
    if (middleX[i] < minX) {
      minX = middleX[i];
    }
    if (middleY[i] < minY) {
      minY = middleY[i];
    }
  }

  uint16_t sumX = 0;
  uint16_t sumY = 0;
  for(uint8_t i=0; i<JOYSTICK_DETECTION_TOTAL; i++) {
    sumX += (middleX[i] - minX);
    sumY += (middleY[i] - minY);
  }

  _middleX = minX + (sumX / JOYSTICK_DETECTION_TOTAL);
  _middleY = minY + (sumY / JOYSTICK_DETECTION_TOTAL);

  #if __DEBUG_LOG_JOYSTICK_HANDLER__
  Serial.print("Origin"), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.println(_middleX);
  Serial.print("Origin"), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.println(_middleY);
  #endif
}

int JoystickHandler::begin() {
  #if __STRICT_MODE__
  verify();
  #endif
  for(int i=0; i < TOTAL_OF_BUTTONS; i++) {
    if (!((JOYSTICK_HIGH_LEVEL_PINS >> i) & 1)) {
      continue;
    }
    #if __PLATFORM_TYPE__ == __PLATFORM_ESP32__
    pinMode(pinOfButtons[i], INPUT_PULLUP);
    continue;
    #endif
    pinMode(pinOfButtons[i], INPUT);
    digitalWrite(pinOfButtons[i], HIGH);
  }
  detect();
  return 0;
}

JoystickAction JoystickHandler::input() {
  JoystickAction message;
  input(&message);
  return message;
}

inline uint16_t adjustAxis(uint16_t z, uint16_t _middleZ, uint16_t _maxZ);

JoystickAction* JoystickHandler::input(JoystickAction* action) {
  if (action == NULL) return action;

  _ordinalNumber += 1;

  #if __DEBUG_LOG_JOYSTICK_HANDLER__
  if (_ordinalNumber < 10) {
    Serial.print("Origin"), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.println(_middleX);
    Serial.print("Origin"), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.println(_middleY);
  }
  #endif

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(JOYSTICK_PIN_X_AXIS);
  uint16_t y = analogRead(JOYSTICK_PIN_Y_AXIS);

  #if __DEBUG_LOG_JOYSTICK_HANDLER__
  char log[32] = { 0 };
  buildJoystickActionLogStr(log, pressed, x, y, _ordinalNumber);
  Serial.print('M'), Serial.print('1'), Serial.print(':'), Serial.print(' '), Serial.println(log);
  #endif

  if (x > _maxX) {
    _maxX = x;
  }
  if (y > _maxY) {
    _maxY = y;
  }

  action->setSource(TX_MSG);
  action->setOrigin(x, y);

  #if __SPACE_SAVING_MODE__
  if (x < _middleX) {
    x = map(x, 0, _middleX, 0, 512);
  } else {
    x = map(x, _middleX, _maxX, 512, 1024);
  }

  if (y < _middleY) {
    y = map(y, 0, _middleY, 0, 512);
  } else {
    y = map(y, _middleY, _maxY, 512, 1024);
  }
  #else
  x = adjustAxis(x, _middleX, _maxX);
  y = adjustAxis(y, _middleY, _maxY);
  #endif

  #if __DEBUG_LOG_JOYSTICK_HANDLER__
  buildJoystickActionLogStr(log, pressed, x, y, _ordinalNumber);
  Serial.print('M'), Serial.print('2'), Serial.print(':'), Serial.print(' '), Serial.println(log);
  #endif

  action->update(pressed, x, y, _ordinalNumber);
  action->setClickingFlags(checkButtonClickingFlags(pressed));

  return action;
}

uint16_t adjustAxis(uint16_t z, uint16_t _middleZ, uint16_t _maxZ) {
  if (z < _middleZ) {
    z = map(z, 0, _middleZ, 0, 512);
  } else {
    z = map(z, _middleZ, _maxZ, 512, 1024);
  }
  return z;
}

uint8_t JoystickHandler::checkArrowKeysToggle(uint16_t x, uint16_t y) {
  uint8_t pressed = 0;
  if (x < _middleX - 255) {
    pressed |= 0b0001; // LEFT
  } else if (x > _middleX + 255) {
    pressed |= 0b1000; // RIGHT
  }
  if (y < _middleY - 255) {
    pressed |= 0b0100; // DOWN
  } else if (y > _middleX + 255) {
    pressed |= 0b0010; // UP
  }

  uint8_t clicked = pressed;
  for (int i = 0; i < 4; i++) {
    uint8_t mask = 1U << i;
    clicked &= (~mask);
    if (pressed & mask) {
      _arrowKeysToggleTrail |= mask;
    } else {
      if (_arrowKeysToggleTrail & mask) {
        _arrowKeysToggleTrail &= (~mask);
        clicked |= mask;
      }
    }
  }

  return clicked;
}

uint16_t JoystickHandler::checkButtonClickingFlags(uint16_t pressed) {
  uint16_t clicked = pressed;
  for (int i = 0; i < TOTAL_OF_BUTTONS; i++) {
    uint16_t mask = 1U << i;
    if (!(CLICK_STYLE_BUTTONS & mask)) {
      continue;
    }
    clicked &= (~mask);
    if (pressed & mask) {
      _clickingTrail |= mask;
    } else {
      if (_clickingTrail & mask) {
        _clickingTrail &= (~mask);
        clicked |= mask;
      }
    }
  }
  return clicked;
}

#if !__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t buttonStates = 0;

  for (int i = 0; i < TOTAL_OF_BUTTONS; i++) {
    if ((JOYSTICK_DISABLED_BUTTONS >> i) & 1) {
      continue;
    }
    buttonStates |= ((digitalRead(pinOfButtons[i]) == ((BUTTON_PRESS_PIN_VALUES >> i) & 1)) ? 1 : 0) << i;
  }

  return buttonStates;
}

#else //__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t pressed = 0;

  if(digitalRead(PIN_UP_BUTTON)==LOW) {
    pressed |= (1 << BIT_UP_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("UP"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_RIGHT_BUTTON)==LOW) {
    pressed |= (1 << BIT_RIGHT_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("RIGHT"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_DOWN_BUTTON)==LOW) {
    pressed |= (1 << BIT_DOWN_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("DOWN"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_LEFT_BUTTON)==LOW) {
    pressed |= (1 << BIT_LEFT_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("LEFT"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_START_BUTTON)==LOW) {
    pressed |= (1 << BIT_START_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("START"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_SELECT_BUTTON)==LOW) {
    pressed |= (1 << BIT_SELECT_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("SELECT"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  if(digitalRead(PIN_ANALOG_BUTTON)==LOW) {
    pressed |= (1 << BIT_ANALOG_BUTTON);
    #if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print("ANALOG"), Serial.print(' '), Serial.print("Button Pressed");
    #endif
  }

  return pressed;
}
#endif //__JOYSTICK_READ_BUTTONS_DEBUG__
