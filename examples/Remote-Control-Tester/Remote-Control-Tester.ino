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

  programSelector.set(&displayHandler);
  programSelector.set(&joystickHandler);

  #if __JOYSTICK_HANDLER_CHECK_ENABLED__
  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, address,
      " Car RC Dashboard"));
  programSelector.add(new ProgramTransmitter(&joystickHandler, &rf24Tranceiver, 0LL,
      " Car RC TX:default"));
  #else//__JOYSTICK_HANDLER_CHECK_ENABLED__
  programSelector.add(new ProgramTransmitter(" Car RC Dashboard",
      &movingResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new ProgramTransmitter(" Car RC TX:default",
      &movingResolver, &displayHandler, &rf24Tranceiver, 0LL));
  #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
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
