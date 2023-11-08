#include <Pokerobo_RCB_master.h>

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver tranceiver;
JoystickHandler joystickHandler;
ProgramSelector programSelector;
SpeedResolver speedResolver;

int count = 0;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  tranceiver.set(&displayHandler);
  tranceiver.set(&speedResolver);
  tranceiver.begin(TX, address);

  joystickHandler.set(&tranceiver);
  joystickHandler.set(&displayHandler);
  joystickHandler.set(&speedResolver);
  joystickHandler.begin();

  programSelector.set(&joystickHandler);
  programSelector.set(&tranceiver, address);
  programSelector.set(&displayHandler);
  programSelector.begin(PROGRAM_MODE_TESTER);
}

void loop() {
  programSelector.check();
}
