#ifndef __PROGRAM_DEFINITION_H__
#define __PROGRAM_DEFINITION_H__

#include "Program_Capsule.h"
#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"

class ProgramTransmitter: public ProgramCapsule {
  public:
    ProgramTransmitter(JoystickHandler* joystickHandler, RF24Tranceiver* tranceiver, uint64_t address);
    int begin();
    int check(void* action);
    int close();
  private:
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver;
    JoystickHandler* _joystickHandler;
};

class ProgramReceiver: public ProgramCapsule {
  public:
    ProgramReceiver(RF24Tranceiver* tranceiver, uint64_t address);
    int begin();
    int check(void* action);
    int close();
  private:
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver;
};

#endif
