#include <Pokerobo_RCB.h>
#include "Program_Accelerometer.h"

#include "ADXL345_Handler.h"
#include "MPU6050_Handler.h"

const uint8_t address = 1;
const uint8_t friendAddress = 0;

JoystickHandler joystickHandler;
MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver;

RF24Tranceiver rf24Tranceiver(&displayHandler, &messageSerializer);
ProgramSelector programSelector(&displayHandler, &joystickHandler);

MPU6050Handler accelerometerHandler;
// ADXL345Handler accelerometerHandler;

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  rf24Tranceiver.begin(RF24_TX, address);

  accelerometerHandler.begin();

  programSelector.add(new ProgramAccelerometer("MPU6050 Commander: %02X",
      &commandResolver, &displayHandler, &rf24Tranceiver, address,
      &accelerometerHandler));
  programSelector.add(new CarCmdConsumer("Car RC Subscriber: %02X",
      &rf24Tranceiver, friendAddress));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
