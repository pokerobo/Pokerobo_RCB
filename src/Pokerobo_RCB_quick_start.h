#ifndef __POKEROBO_RCB_QUICK_START_H__
#define __POKEROBO_RCB_QUICK_START_H__

#include "Pokerobo_RCB_main_Uno.h"

JoystickHandler joystickHandler;
MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver;

RF24Tranceiver rf24Tranceiver(&displayHandler, &messageSerializer);
ProgramSelector programSelector(&displayHandler, &joystickHandler);

class QuickStartLauncher {
  public:
    void begin(uint8_t address=1, uint8_t friendAddress=1);
    void check();
};

void QuickStartLauncher::begin(uint8_t address, uint8_t friendAddress) {
  joystickHandler.begin();
  displayHandler.begin();

  programSelector.add(new CarCmdProducer("Car RC Commander: %02X",
      &commandResolver, &displayHandler, &rf24Tranceiver, address));
  programSelector.add(new CarCmdConsumer("Car RC Subscriber: %02X",
      &rf24Tranceiver, friendAddress));
  programSelector.add(new ProgramDeviceInfo("Device Information",
      &displayHandler));

  programSelector.begin();
}

void QuickStartLauncher::check() {
  programSelector.check();
}

#endif
