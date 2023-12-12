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
  rf24Tranceiver.set(&movingResolver);
  rf24Tranceiver.begin(RF24_TX, address);

  joystickHandler.set(&rf24Tranceiver);
  joystickHandler.set(&displayHandler);
  joystickHandler.set(&movingResolver);
  joystickHandler.begin();

  programSelector.set(&joystickHandler);
  programSelector.set(&displayHandler);

  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, 0LL));

  programSelector.begin(PROGRAM_MODE_PLAYER);
}

void loop() {
  programSelector.check();
}
