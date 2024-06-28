#include <Pokerobo_RCB.h>

const uint8_t address = 1; // from 1 (0x01) -> 255 (0xFF)

MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver(true);
MovingCommandPacket commandBuffer;

RF24Tranceiver rf24Tranceiver;
JoystickHandler joystickHandler;
ProgramSelector programSelector;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();
  joystickHandler.begin();

  rf24Tranceiver.set(&displayHandler);
  rf24Tranceiver.set(&messageSerializer);
  rf24Tranceiver.begin(RF24_TX, address);

  programSelector.set(&displayHandler);
  programSelector.set(&joystickHandler);

  programSelector.add(new ProgramTransmitter("Car RC Dashboard",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter("Car RC TX:default",
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 0));

  programSelector.add(new ProgramReceiver("Car RC RX:default",
      &rf24Tranceiver, 0));
  programSelector.add(new ProgramReceiver("Car RC RX:18580907",
      &rf24Tranceiver, 7));
  programSelector.add(new ProgramReceiver("Car RC RX:18580908",
      &rf24Tranceiver, 8));
  programSelector.add(new ProgramDeviceInfo("Device Information",
      &displayHandler));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
