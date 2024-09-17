#include <Pokerobo_RCB.h>

const uint8_t address = 1; // from 1 (0x01) -> 255 (0xFF)
const uint8_t friendAddress = 0;

JoystickHandler joystickHandler;
MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver(true);
MovingCommandPacket commandBuffer;

RF24Tranceiver rf24Tranceiver(&displayHandler, &messageSerializer);
ProgramSelector programSelector(&displayHandler, &joystickHandler);

ExecutionTimer executionTimer;

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  programSelector.add(new ProgramTransmitter("Car RC Dashboard",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));

  programSelector.add(new ProgramTransmitter("Car RC TX:friend",
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, friendAddress));
  programSelector.add(new ProgramReceiver("Car RC RX:friend",
      &rf24Tranceiver, friendAddress));

  programSelector.add(new ProgramDeviceInfo("Device Information",
      &displayHandler));

  programSelector.setDelayAmount(0);

  programSelector.begin();
}

void loop() {
  executionTimer.start();
  programSelector.check();
  delay(executionTimer.remain(100));
}
