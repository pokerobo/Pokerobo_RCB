#ifndef __PROGRAM_DEFINITION_H__
#define __PROGRAM_DEFINITION_H__

#include "Program_Capsule.h"
#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"
#include "Message_Exchange.h"
#include "Message_Renderer.h"
#include "Moving_Resolver.h"

class ProgramTransmitter: public ProgramCapsule {
  public:
    #if __JOYSTICK_HANDLER_CHECK_ENABLED__
    ProgramTransmitter(JoystickHandler* joystickHandler,
      RF24Tranceiver* tranceiver,
      uint64_t address,
      char* title);
    #else
    ProgramTransmitter(MessageSender* messageSender,
      MessageRenderer* messageRenderer,
      MovingResolver* movingResolver,
      RF24Tranceiver* tranceiver,
      uint64_t address,
      char* title);
    #if MULTIPLE_SENDERS_SUPPORTED
    bool add(MessageSender* messageSender);
    #endif
    void set(MessageSender* messageSender);
    void set(MessageRenderer* messageRenderer);
    void set(MovingResolver* movingResolver);
    #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
    char* getTitle();
    int begin();
    int check(void* action);
    int close();
  private:
    char* _title;
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver;
    #if __JOYSTICK_HANDLER_CHECK_ENABLED__
    JoystickHandler* _joystickHandler;
    #else//__JOYSTICK_HANDLER_CHECK_ENABLED__
    TransmissionCounter _counter;
    MessageSender* _messageSender = NULL;
    #if MULTIPLE_SENDERS_SUPPORTED
    MessageSender* _messageSenders[MESSAGE_EXCHANGE_MAX] = {};
    uint8_t _messageSendersTotal = 0;
    #endif//MULTIPLE_SENDERS_SUPPORTED
    MessageRenderer* _messageRenderer = NULL;
    MovingResolver* _movingResolver = NULL;
    #endif//__JOYSTICK_HANDLER_CHECK_ENABLED__
};

class ProgramReceiver: public ProgramCapsule {
  public:
    ProgramReceiver(RF24Tranceiver* tranceiver, uint64_t address, char* title);
    char* getTitle();
    int begin();
    int check(void* action);
    int close();
  private:
    char* _title;
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver;
};

#endif
