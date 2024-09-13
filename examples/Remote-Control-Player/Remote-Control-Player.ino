#include <Pokerobo_RCB.h>

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

  programSelector.add(new CarCmdProducer("Car RC TX: %02X",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new CarCmdConsumer("Car RC RX: %02X",
      &rf24Tranceiver, friendAddress));
  programSelector.add(new ProgramDeviceInfo("Device Information",
      &displayHandler));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
