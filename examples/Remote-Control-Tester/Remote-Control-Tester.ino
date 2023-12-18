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

  #if __JOYSTICK_HANDLER_CHECK_ENABLED__
  joystickHandler.set(&rf24Tranceiver);
  joystickHandler.set(&displayHandler);
  joystickHandler.set(&movingResolver);
  #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
  joystickHandler.begin();

  programSelector.set(&joystickHandler);
  programSelector.set(&displayHandler);

  #if __JOYSTICK_HANDLER_CHECK_ENABLED__
  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, address,
      " Car RC Dashboard"));
  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, 0LL,
      " Car RC TX:default"));
  #else//__JOYSTICK_HANDLER_CHECK_ENABLED__
  programSelector.add(new ProgramTransmitter(&rf24Tranceiver, &displayHandler, &movingResolver,
      &rf24Tranceiver, address, " Car RC Dashboard"));
  programSelector.add(new ProgramTransmitter(&rf24Tranceiver, &displayHandler, &movingResolver,
      &rf24Tranceiver, 0LL, " Car RC TX:default"));
  #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
  programSelector.add(new ProgramReceiver(&rf24Tranceiver, 0LL,
      " Car RC RX:default"));
  programSelector.add(new ProgramReceiver(&rf24Tranceiver, 0x18580831LL,
      " Car RC RX:18580831"));
  programSelector.add(new ProgramReceiver(&rf24Tranceiver, 0x18580902LL,
      " Car RC RX:18580902"));
  programSelector.add(new ProgramReceiver(&rf24Tranceiver, 0x18580903LL,
      " Car RC RX:18580903"));

  programSelector.begin(PROGRAM_MODE_TESTER);
}

void loop() {
  programSelector.check();
}
