#include <Pokerobo_RCB_main_Nano.h>

const uint8_t address = 1;
const uint8_t friendAddress = 1;

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

  programSelector.add(new CarCmdProducer("Car RC Commander: %02X",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new CarCmdConsumer("Car RC Subscriber: %02X",
      &rf24Tranceiver, friendAddress));

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
