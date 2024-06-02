#ifndef __POKEROBO_RCB_PROGRAM_ENTRYPOINT_H__
#define __POKEROBO_RCB_PROGRAM_ENTRYPOINT_H__

#include "Program_Capsule.h"
#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"
#include "Message_Exchange.h"
#include "Message_Renderer.h"
#include "Message_Resolver.h"

#define MULTIPLE_SENDERS_SUPPORTED false

class ProgramTransmitter: public ProgramCapsule {
  public:
    ProgramTransmitter(char* title,
      CommandResolver* commandResolver, MessageRenderer* messageRenderer,
      RF24Tranceiver* tranceiver, uint64_t address): ProgramTransmitter(title, NULL,
        commandResolver, messageRenderer, tranceiver, address) {};
    ProgramTransmitter(char* title,
      CommandPacket* commandBuffer, CommandResolver* commandResolver, MessageRenderer* messageRenderer,
      RF24Tranceiver* tranceiver, uint64_t address);
    #if MULTIPLE_SENDERS_SUPPORTED
    bool add(MessageSender* messageSender);
    #endif
    void set(MessageSender* messageSender);
    void set(MessageRenderer* messageRenderer);
    void set(CommandResolver* commandResolver);
    void set(CommandPacket* commandBuffer);
    bool hasCommandBuffer();
    uint8_t getId();
    char* getTitle();
    int begin();
    int check(void* action, void* command=NULL);
    int close();
  private:
    static const uint8_t _applicationId = 1;
    char* _title;
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver = NULL;
    TransmissionCounter _counter;
    MessageSender* _messageSender = NULL;
    #if MULTIPLE_SENDERS_SUPPORTED
    MessageSender* _messageSenders[MESSAGE_EXCHANGE_MAX] = {};
    uint8_t _messageSendersTotal = 0;
    #endif//MULTIPLE_SENDERS_SUPPORTED
    MessageRenderer* _messageRenderer = NULL;
    CommandResolver* _commandResolver = NULL;
    CommandPacket* _commandBuffer = NULL;
};

class ProgramReceiver: public ProgramCapsule {
  public:
    ProgramReceiver(char* title,
      RF24Tranceiver* tranceiver, uint64_t address);
    uint8_t getId();
    char* getTitle();
    int begin();
    int check(void* action, void* command=NULL);
    int close();
  private:
    static const uint8_t _applicationId = 2;
    char* _title;
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    RF24Tranceiver* _rf24Tranceiver;
};

#endif