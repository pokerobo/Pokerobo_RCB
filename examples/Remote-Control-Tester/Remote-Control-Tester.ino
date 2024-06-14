#include <Pokerobo_RCB_master.h>

const uint8_t address = 1;

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

  rf24Tranceiver.set(&displayHandler);
  rf24Tranceiver.set(&messageSerializer);
  rf24Tranceiver.begin(RF24_TX, address);

  joystickHandler.begin();

  programSelector.set(&displayHandler);
  programSelector.set(&joystickHandler);

  programSelector.add(new ProgramTransmitter(" Car RC Dashboard",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter(" Car RC TX:default",
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 0));
  programSelector.add(new ProgramTransmitter(" Car RC TX:18580908",
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 8));

  programSelector.add(new ProgramReceiver(" Car RC RX:default",
      &rf24Tranceiver, 0));
  programSelector.add(new ProgramReceiver(" Car RC RX:18580908",
      &rf24Tranceiver, 8));

  programSelector.begin(PROGRAM_MODE_TESTER);
}

void loop() {
  programSelector.check();
}
