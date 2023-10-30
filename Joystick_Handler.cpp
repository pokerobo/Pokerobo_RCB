#include "Joystick_Handler.h"

#ifndef JOYSTICK_DETECTION_TOTAL
#define JOYSTICK_DETECTION_TOTAL  20
#endif//JOYSTICK_DETECTION_TOTAL

static int JoystickHandler::pinOfButtons[] = {
  PIN_UP_BUTTON,
  PIN_RIGHT_BUTTON,
  PIN_DOWN_BUTTON,
  PIN_LEFT_BUTTON,
  PIN_START_BUTTON,
  PIN_SELECT_BUTTON,
  PIN_ANALOG_BUTTON
};

#if __STRICT_MODE__
static void JoystickHandler::verify() {
  pinOfButtons[BIT_UP_BUTTON] = PIN_UP_BUTTON;
  pinOfButtons[BIT_RIGHT_BUTTON] = PIN_RIGHT_BUTTON;
  pinOfButtons[BIT_DOWN_BUTTON] = PIN_DOWN_BUTTON;
  pinOfButtons[BIT_LEFT_BUTTON] = PIN_LEFT_BUTTON;
  pinOfButtons[BIT_START_BUTTON] = PIN_START_BUTTON;
  pinOfButtons[BIT_SELECT_BUTTON] = PIN_SELECT_BUTTON;
  pinOfButtons[BIT_ANALOG_BUTTON] = PIN_ANALOG_BUTTON;
}
#endif

JoystickHandler::JoystickHandler(MessageSender* messageSender, MessageRenderer* messageRenderer) {
  _messageRenderer = messageRenderer;
  add(messageSender);
}

void JoystickHandler::detect() {
  uint16_t middleX[JOYSTICK_DETECTION_TOTAL] = {};
  uint16_t middleY[JOYSTICK_DETECTION_TOTAL] = {};
  uint16_t minX = 1024;
  uint16_t minY = 1024;

  for(uint8_t i=0; i<JOYSTICK_DETECTION_TOTAL; i++) {
    delay(50);
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

  #if __RUNNING_LOG_ENABLED__
    Serial.print("Origin "), Serial.print("X"), Serial.print(": "), Serial.println(_middleX);
    Serial.print("Origin "), Serial.print("Y"), Serial.print(": "), Serial.println(_middleY);
  #endif
}

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

int JoystickHandler::check() {
  _count += 1;

#if __RUNNING_LOG_ENABLED__
  if (_count < 10) {
    Serial.print("Middle "), Serial.print("X"), Serial.print(": "), Serial.println(_middleX);
    Serial.print("Middle "), Serial.print("Y"), Serial.print(": "), Serial.println(_middleY);
  }
#endif

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(PIN_JOYSTICK_X_AXIS);
  uint16_t y = analogRead(PIN_JOYSTICK_Y_AXIS);

#if __RUNNING_LOG_ENABLED__
  char log[32] = "";
  sprintf(log, "%d,%d,%d", pressed, x, y);
  Serial.print("M1"), Serial.print(": "), Serial.println(log);
#endif

  uint16_t originX = x;
  if (x < _middleX) {
    x = map(x, 0, _middleX, 0, 512);
  } else {
    x = map(x, _middleX, JOYSTICK_MAX_X, 512, 1024);
  }

  uint16_t originY = y;
  if (y < _middleY) {
    y = map(y, 0, _middleY, 0, 512);
  } else {
    y = map(y, _middleY, JOYSTICK_MAX_Y, 512, 1024);
  }

  JoystickAction message(pressed, x, y, _count);
  message.setOrigin(originX, originY);
  if (_messageRenderer != NULL) {
    _messageRenderer->render(&message);
  }

#if __RUNNING_LOG_ENABLED__
    sprintf(log, "%d,%d,%d", pressed, x, y);
    Serial.print("M2"), Serial.print(": "), Serial.println(log);
#endif

  if (isChanged(x, y, pressed)) {
    int8_t countNulls = 0, sumFails = 0, sumOk = 0;
    for(int i=0; i<_messageSendersTotal; i++) {
      int8_t status = invoke(_messageSenders[i], i+1, NULL, 0, &message);
      if (status > 0) {
        sumOk += status;
      } else if (status < 0) {
        sumFails += status;
      } else {
        countNulls++;
      }
    }

    return 2;
  } else {
    return 1;
  }
}

bool JoystickHandler::isChanged(int16_t x, int16_t y, uint32_t buttons) {
#if JOYSTICK_CHECKING_CHANGE
  return !(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || buttons;
#endif
  return true;
}

byte JoystickHandler::invoke(MessageSender* messageSender, uint8_t index, const void* buf, uint8_t len, MessagePacket* packet) {
  if (messageSender != NULL) {
    uint8_t code = 1 << index;

    bool ok = false;
    if (packet != NULL) {
      ok = messageSender->write(packet);
    } else {
      ok = messageSender->write(buf, len);
    }

#if __RUNNING_LOG_ENABLED__
    Serial.print("#"), Serial.print(_count), Serial.print("->"), Serial.print(index), Serial.print(": ");
    if (ok) {
      Serial.println("v");
    } else {
      Serial.println("x");
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

#if !__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t buttonStates = 0;

  for (int i = 0; i < TOTAL_OF_BUTTONS; i++) {
    buttonStates |= ((digitalRead(pinOfButtons[i]) == LOW) ? 1 : 0) << i;
  }

  return buttonStates;
}

#else //__JOYSTICK_READ_BUTTONS_DEBUG__

uint16_t JoystickHandler::readButtonStates() {
  uint16_t pressed = 0;

  if(digitalRead(PIN_UP_BUTTON)==LOW) {
    pressed |= (1 << BIT_UP_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("UP"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_RIGHT_BUTTON)==LOW) {
    pressed |= (1 << BIT_RIGHT_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("RIGHT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_DOWN_BUTTON)==LOW) {
    pressed |= (1 << BIT_DOWN_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("DOWN"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_LEFT_BUTTON)==LOW) {
    pressed |= (1 << BIT_LEFT_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("LEFT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_START_BUTTON)==LOW) {
    pressed |= (1 << BIT_START_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("START"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_SELECT_BUTTON)==LOW) {
    pressed |= (1 << BIT_SELECT_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("SELECT"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  if(digitalRead(PIN_ANALOG_BUTTON)==LOW) {
    pressed |= (1 << BIT_ANALOG_BUTTON);
#if __RUNNING_LOG_ENABLED__
    Serial.print("ANALOG"), Serial.print(" "), Serial.print("Button Pressed");
#endif
  }

  return pressed;
}
#endif //__JOYSTICK_READ_BUTTONS_DEBUG__
