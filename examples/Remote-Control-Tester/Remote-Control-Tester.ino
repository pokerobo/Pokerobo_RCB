#include <Pokerobo_RCB_master.h>

const uint64_t address = 0x18580901LL;

MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
RF24Tranceiver rf24Tranceiver;
JoystickHandler joystickHandler;
ProgramSelector programSelector;
MovingResolver movingResolver;

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
      &movingResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter(" Car RC TX:default",
      &movingResolver, &displayHandler, &rf24Tranceiver, 0LL));
  programSelector.add(new ProgramReceiver(" Car RC RX:default",
      &rf24Tranceiver, 0LL));
  programSelector.add(new ProgramReceiver(" Car RC RX:18580831",
      &rf24Tranceiver, 0x18580831LL));
  programSelector.add(new ProgramReceiver(" Car RC RX:18580901",
      &rf24Tranceiver, 0x18580901LL));
  programSelector.add(new ProgramReceiver(" Car RC RX:18580902",
      &rf24Tranceiver, 0x18580902LL));
  programSelector.add(new ProgramReceiver(" Car RC RX:18580903",
      &rf24Tranceiver, 0x18580903LL));

  programSelector.begin(PROGRAM_MODE_TESTER);
}

void loop() {
  programSelector.check();
}
