#ifndef __RUNNING_LOG_ENABLED__
#define __RUNNING_LOG_ENABLED__       1
#endif

#define __FUNDUINO_JOYSTICK_SHIELD__  1

#if __FUNDUINO_JOYSTICK_SHIELD__
#define JOYSTICK_MAX_X   723
#define JOYSTICK_MAX_Y   723
#else
#define JOYSTICK_MAX_X   950
#define JOYSTICK_MAX_Y   950
#endif

#define MIN_BOUND_X      512 - 64
#define MAX_BOUND_X      512 + 64
#define MIN_BOUND_Y      512 - 64
#define MAX_BOUND_Y      512 + 64

#define BIT_UP_BUTTON     1 << 0
#define BIT_RIGHT_BUTTON  1 << 1
#define BIT_DOWN_BUTTON   1 << 2
#define BIT_LEFT_BUTTON   1 << 3
#define BIT_START_BUTTON  1 << 4
#define BIT_SELECT_BUTTON 1 << 5
#define BIT_ANALOG_BUTTON 1 << 6

#define PIN_UP_BUTTON     2 // A
#define PIN_RIGHT_BUTTON  3 // B
#define PIN_DOWN_BUTTON   4 // C
#define PIN_LEFT_BUTTON   5 // D
#define PIN_START_BUTTON  6 // F
#define PIN_SELECT_BUTTON 7 // E
#define PIN_ANALOG_BUTTON 8 // JOYSTICK

#include "RF24_Tranceiver.h"

int x_axis = A0;
int y_axis = A1;

int buttons[] = {
  PIN_UP_BUTTON,
  PIN_RIGHT_BUTTON,
  PIN_DOWN_BUTTON,
  PIN_LEFT_BUTTON,
  PIN_START_BUTTON,
  PIN_SELECT_BUTTON,
  PIN_ANALOG_BUTTON
};

uint32_t count = 0;

RF24Tranceiver transmitterInstance;
RF24Tranceiver* transmitter = &transmitterInstance;

void setup() {
  for(int i; i <7 ; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);  
  }

  Serial.begin(57600);

  transmitter->begin();
}

void loop() {
  count += 1;

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(x_axis);
  uint16_t y = analogRead(y_axis);

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

  if (!(MIN_BOUND_X < x && x < MAX_BOUND_X && MIN_BOUND_Y < y && y < MAX_BOUND_Y) || pressed) {
    uint8_t msg[12] = {};
    encodeMessage(msg, "JS", pressed, x, y, count);
    bool ok = transmitter->write(msg, sizeof(msg));
    if (ok) {
#if __RUNNING_LOG_ENABLED__
      Serial.print("->"), Serial.println("OK");
#endif
    } else {
#if __RUNNING_LOG_ENABLED__
      Serial.print("->"), Serial.println("FAIL");
#endif
    }
    delay(50);
  } else {
    delay(5);
  }
}

uint8_t* encodeMessage(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t flags) {
  buf[0] = cmd[0];
  buf[1] = cmd[1];
  encodeInteger(&buf[2], pressed);
  encodeInteger(&buf[4], x);
  encodeInteger(&buf[6], y);
  encodeInteger(&buf[8], flags);
  return buf;
}

uint8_t* encodeInteger(uint8_t* store, uint16_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  return store;
}

uint8_t* encodeInteger(uint8_t* store, uint32_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  store[1] = (value >> 16) & 0xff;
  store[1] = (value >> 24) & 0xff;
  return store;
}

uint16_t readButtonStates() {
  uint16_t buttonStates = 0;

  for (int i = 0; i < 7; i++) {
    buttonStates |= ((digitalRead(buttons[i]) == LOW) ? 1 : 0) << i;
  }

  return buttonStates;
}

uint16_t readButtonStatesForDebug() {
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
