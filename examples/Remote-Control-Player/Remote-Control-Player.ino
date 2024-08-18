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

  programSelector.add(new ProgramTransmitter("Car RC Dashboard",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter("Car RC TX:friend",
      &commandResolver, &displayHandler, &rf24Tranceiver, friendAddress));
  programSelector.add(new ProgramReceiver("Car RC RX:friend",
      &rf24Tranceiver, friendAddress));
  programSelector.add(new ProgramDeviceInfo("Device Information",
      &displayHandler));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
