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

#if __JOYSTICK_FUNDUINO_SHIELD__
#define BUTTON_PRESS_PIN_VALUES   0b0000000
#else
#define BUTTON_PRESS_PIN_VALUES   0b0111111
#endif

#define CLICK_STYLE_BUTTONS       MASK_START_BUTTON | MASK_SELECT_BUTTON | MASK_ANALOG_BUTTON

int JoystickHandler::pinOfButtons[] = {
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
    middleX[i] = analogRead(PIN_JOYSTICK_X_AXIS);
    middleY[i] = analogRead(PIN_JOYSTICK_Y_AXIS);
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

void JoystickHandler::set(MessageSender* messageSender) {
  _messageSender = messageSender;
}

#if MULTIPLE_SENDERS_SUPPORTED
bool JoystickHandler::add(MessageSender* messageSender) {
  if (messageSender == NULL) {
    return false;
  }
  if (_messageSendersTotal > MESSAGE_SENDER_MAX) {
    return false;
  }
#if __STRICT_MODE__
  for(int i=0; i<_messageSendersTotal; i++) {
    if (_messageSenders[i] == messageSender) {
      return false;
    }
  }
#endif
  _messageSenders[_messageSendersTotal++] = messageSender;
  return true;
}
#endif//MULTIPLE_SENDERS_SUPPORTED

void JoystickHandler::set(MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
}

void JoystickHandler::set(SpeedResolver* speedResolver) {
  _speedResolver = speedResolver;
}

int JoystickHandler::begin() {
#if __STRICT_MODE__
  verify();
#endif
  for(int i; i < TOTAL_OF_BUTTONS; i++) {
    pinMode(pinOfButtons[i], INPUT);
    digitalWrite(pinOfButtons[i], HIGH);
  }
  detect();
}

inline void _adjustCounter(TransmissionCounter *counter);

int JoystickHandler::check(JoystickAction* action) {
    _counter.ordinalNumber += 1;
    _counter.packetLossTotal += 1;

  if (action == NULL) {
    JoystickAction message = input();
    action = &message;
  }

  SpeedPacket speedPacket;

  if (_speedResolver != NULL) {
    _speedResolver->resolve(&speedPacket, action);
  }

#if JOYSTICK_CHECKING_CHANGE
  if (!isChanged(action)) {
    return 1;
  }
#endif

  if (_messageSender != NULL) {
    bool ok = _messageSender->write(action);
    if (ok) {
      _counter.packetLossTotal -= 1;
    }
  }

#if MULTIPLE_SENDERS_SUPPORTED
  int8_t countNulls = 0, sumFails = 0, sumOk = 0;
  for(int i=0; i<_messageSendersTotal; i++) {
    int8_t status = invoke(_messageSenders[i], i+1, NULL, 0, action);
    if (status > 0) {
      sumOk += status;
    } else if (status < 0) {
      sumFails += status;
    } else {
      countNulls++;
    }
  }
#endif

  if (_messageRenderer != NULL) {
    _messageRenderer->render(action, &speedPacket, &_counter);
  }

  _adjustCounter(&_counter);

  return 2;
}

void _adjustCounter(TransmissionCounter *counter) {
  if (counter->ordinalNumber >= 999999UL) {
    counter->ordinalNumber = 0;
    counter->packetLossTotal = 0;
  }
}

JoystickAction JoystickHandler::input() {
  JoystickAction message;
  input(&message);
  return message;
}

inline uint16_t adjustAxis(uint16_t z, uint16_t _middleZ, uint16_t _maxZ);

JoystickAction* JoystickHandler::input(JoystickAction* action) {
  if (action == NULL) return action;

#if __DEBUG_LOG_JOYSTICK_HANDLER__
  if (_counter.ordinalNumber < 10) {
    Serial.print("Origin"), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.println(_middleX);
    Serial.print("Origin"), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.println(_middleY);
  }
#endif

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(PIN_JOYSTICK_X_AXIS);
  uint16_t y = analogRead(PIN_JOYSTICK_Y_AXIS);

#if __DEBUG_LOG_JOYSTICK_HANDLER__
  char log[32] = { 0 };
  char fmt[12] = { '%', 'd', ',', '%', 'd', ',', '%', 'd', ',', '%', 'd', '\0' };
  sprintf(log, fmt, pressed, x, y, _counter.ordinalNumber);
  Serial.print("M1"), Serial.print(':'), Serial.print(' '), Serial.println(log);
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
    sprintf(log, fmt, pressed, x, y, _counter.ordinalNumber);
    Serial.print("M2"), Serial.print(':'), Serial.print(' '), Serial.println(log);
#endif

  action->update(pressed, x, y, _counter.ordinalNumber);
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

#if JOYSTICK_CHECKING_CHANGE
bool JoystickHandler::isChanged(JoystickAction* msg) {
  int16_t x = msg->getX();
  int16_t y = msg->getY();
  uint32_t buttons = msg->getPressingFlags();
  return !(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || buttons;
}
#endif

#if MULTIPLE_SENDERS_SUPPORTED
byte JoystickHandler::invoke(MessageSender* messageSender, uint8_t index, const void* buf, uint8_t len, MessagePacket* packet) {
  if (messageSender != NULL) {
    uint8_t code = 1 << index;

    bool ok = false;
    if (packet != NULL) {
      ok = messageSender->write(packet);
    } else {
      ok = messageSender->write(buf, len);
    }

#if __DEBUG_LOG_JOYSTICK_HANDLER__
    Serial.print('#'), Serial.print(_counter.ordinalNumber), Serial.print("->"), Serial.print(index), Serial.print(": ");
    if (ok) {
      Serial.println('v');
    } else {
      Serial.println('x');
    }
#endif

    if (ok) {
      return code;
    } else {
      return -code;
    }
  }
  return 0;
}
#endif

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
