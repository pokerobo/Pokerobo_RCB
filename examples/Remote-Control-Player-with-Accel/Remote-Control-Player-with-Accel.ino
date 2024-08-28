#include <Pokerobo_RCB.h>
#include "Program_Accelerometer.h"

const uint8_t address = 1;
const uint8_t friendAddress = 0;

JoystickHandler joystickHandler;
MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver;

RF24Tranceiver rf24Tranceiver(&displayHandler, &messageSerializer);
ProgramSelector programSelector(&displayHandler, &joystickHandler);

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  rf24Tranceiver.begin(RF24_TX, address);

  programSelector.add(new ProgramTransmitter("Car RC Dashboard",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramAccelerometer("Car RC Accelerometer",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
