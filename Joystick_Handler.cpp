#include "Joystick_Handler.h"

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
static void JoystickHandler::init() {
  pinOfButtons[BIT_UP_BUTTON] = PIN_UP_BUTTON;
  pinOfButtons[BIT_RIGHT_BUTTON] = PIN_RIGHT_BUTTON;
  pinOfButtons[BIT_DOWN_BUTTON] = PIN_DOWN_BUTTON;
  pinOfButtons[BIT_LEFT_BUTTON] = PIN_LEFT_BUTTON;
  pinOfButtons[BIT_START_BUTTON] = PIN_START_BUTTON;
  pinOfButtons[BIT_SELECT_BUTTON] = PIN_SELECT_BUTTON;
  pinOfButtons[BIT_ANALOG_BUTTON] = PIN_ANALOG_BUTTON;
}
#endif

JoystickHandler::JoystickHandler(MessageSender* messageSender) {
  add(messageSender);
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
  init();
#endif
  for(int i; i <7 ; i++) {
    pinMode(pinOfButtons[i], INPUT);
    digitalWrite(pinOfButtons[i], HIGH);
  }
}

int JoystickHandler::check() {
  _count += 1;

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(PIN_JOYSTICK_X_AXIS);
  uint16_t y = analogRead(PIN_JOYSTICK_Y_AXIS);

#if __RUNNING_LOG_ENABLED__
    char log[32] = "";
    sprintf(log, "%d,%d,%d", pressed, x, y);
    Serial.print("M1"), Serial.print(": "), Serial.println(log);
#endif

  if (x < JOYSTICK_MID_X) {
    x = map(x, 0, JOYSTICK_MID_X, 0, 512);
  } else {
    x = map(x, JOYSTICK_MID_X, JOYSTICK_MAX_X, 512, 1024);
  }

  if (y < JOYSTICK_MID_Y) {
    y = map(y, 0, JOYSTICK_MID_Y, 0, 512);
  } else {
    y = map(y, JOYSTICK_MID_Y, JOYSTICK_MAX_Y, 512, 1024);
  }

#if __RUNNING_LOG_ENABLED__
    sprintf(log, "%d,%d,%d", pressed, x, y);
    Serial.print("M2"), Serial.print(": "), Serial.println(log);
#endif

  if (isChanged(x, y, pressed)) {
    JoystickAction message(pressed, x, y, _count);
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
  return !(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || buttons;
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

  for (int i = 0; i < 7; i++) {
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
