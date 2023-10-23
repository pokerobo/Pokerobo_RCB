#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define __RUNNING_LOG_ENABLED__       1
#define __RF24_BINARY_ENCODING__      1
#define __FUNDUINO_JOYSTICK_SHIELD__  0

#if __FUNDUINO_JOYSTICK_SHIELD__
#define JOYSTICK_MAX_X   750
#define JOYSTICK_MAX_Y   750
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

#define CE_PIN  9
#define CSN_PIN 10

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

const uint64_t address = 0xE8E8F0F0E1LL;

uint32_t count = 0;

RF24 radio(CE_PIN,CSN_PIN);

void setup() {
  for(int i; i <7 ; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);  
  }

  Serial.begin(57600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  count += 1;

  uint16_t pressed = readButtonStates();

  uint16_t x = analogRead(x_axis);
  uint16_t y = analogRead(y_axis);

#if __RUNNING_LOG_ENABLED__
    char log[32] = "";
    sprintf(log, "M1: %d,%d,%d", pressed, x, y);
    Serial.println(log);
#endif

  x = map(x, 0, JOYSTICK_MAX_X, 0, 1024);
  y = map(y, 0, JOYSTICK_MAX_Y, 0, 1024);

#if __RUNNING_LOG_ENABLED__
    sprintf(log, "M2: %d,%d,%d", pressed, x, y);
    Serial.println(log);
#endif

  if (x < MIN_BOUND_X || x > MAX_BOUND_X || y < MIN_BOUND_Y || y > MAX_BOUND_Y) {
#if __RF24_BINARY_ENCODING__
    uint8_t msg[12] = {};
    msg[0] = 'J';
    msg[1] = 'S';
    encodeInteger(&msg[2], pressed);
    encodeInteger(&msg[4], x);
    encodeInteger(&msg[6], y);
    encodeInteger(&msg[8], count);
#else
    char msg[32] = "";
    sprintf(msg, "JT,%d,%d,%d,%d", pressed, x, y, count);
#if __RUNNING_LOG_ENABLED__
    Serial.println(msg);
#endif
#endif
    radio.write(msg, sizeof(msg));
    delay(5);
  } else {
    delay(3);
  }
}

void encodeInteger(uint8_t* store, uint16_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
}

void encodeInteger(uint8_t* store, uint32_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  store[1] = (value >> 16) & 0xff;
  store[1] = (value >> 24) & 0xff;
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
