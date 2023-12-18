#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include "Commons.h"
#include "Joystick_Action.h"
#include "Message_Exchange.h"
#include "Message_Processor.h"
#include "Message_Renderer.h"
#include "Moving_Resolver.h"

#define RF24_DEFAULT_ADDRESS 0x123456789ABCDEF0LL

#define MESSAGE_RENDERERS_LIMIT   7
#define MULTIPLE_RENDERERS_SUPPORTED false

typedef enum { RF24_TX = 0, RF24_RX } tranceiver_t;
typedef enum { ACK_OK = 0, ACK_FAILED, MESSAGE_NULL, TRANSMITTER_NULL } rf24_tx_status_t;

class RF24Transmitter: public MessageSender {
  public:
    int begin(uint64_t address=RF24_DEFAULT_ADDRESS, void* radio=NULL);
    bool write(const void* buf, uint8_t len);
    bool write(MessagePacket* packet);
    rf24_tx_status_t getStatus();
    void reset();
  private:
    void* _transmitter = NULL;
    rf24_tx_status_t _status;
};

class RF24Receiver {
  public:
    int begin(uint64_t address=RF24_DEFAULT_ADDRESS, void* radio = NULL);
    int check();
    void reset();
    void set(MessageProcessor* messageProcessor);
    void set(MessageRenderer* messageRenderer);
    #if MULTIPLE_RENDERERS_SUPPORTED
    bool add(MessageRenderer* messageRenderer);
    #endif
    #if RECALCULATING_MOVING_COMMAND
    void set(MovingResolver* movingResolver);
    #endif
  protected:
    bool available();
    #if MULTIPLE_RENDERERS_SUPPORTED
    byte invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message,
        MovingCommand* movingCommand, TransmissionCounter* counter);
    #endif
  private:
    void* _receiver = NULL;
    TransmissionCounter _counter;
    uint32_t _discontinuityCount = 0;
    MessageProcessor* _messageProcessor = NULL;
    MessageRenderer* _messageRenderer = NULL;
    #if MULTIPLE_RENDERERS_SUPPORTED
    MessageRenderer* _messageRenderers[MESSAGE_RENDERERS_LIMIT] = {};
    uint8_t _messageRenderersTotal = 0;
    #endif
    #if RECALCULATING_MOVING_COMMAND
    MovingResolver* _movingResolver = NULL;
    #endif
};

class RF24Tranceiver: public RF24Transmitter, public RF24Receiver {
  public:
    int begin(tranceiver_t mode=RF24_TX, uint64_t address=RF24_DEFAULT_ADDRESS);
    void reset(tranceiver_t mode=RF24_TX);
};

#endif
