#include <Pokerobo_RCB_master.h>

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver rf24Tranceiver;
JoystickHandler joystickHandler;
ProgramSelector programSelector;
MovingResolver movingResolver;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  rf24Tranceiver.set(&displayHandler);
  #if RECALCULATING_MOVING_COMMAND
  rf24Tranceiver.set(&movingResolver);
  #endif
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

  programSelector.begin(PROGRAM_MODE_PLAYER);
}

void loop() {
  programSelector.check();
}
