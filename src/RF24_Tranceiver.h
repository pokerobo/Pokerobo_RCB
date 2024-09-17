#ifndef __POKEROBO_RCB_RF24_TRANCEIVER_H__
#define __POKEROBO_RCB_RF24_TRANCEIVER_H__

#include "Commons.h"
#include "Joystick_Action.h"
#include "Message_Exchange.h"
#include "Message_Monitor.h"
#include "Message_Processor.h"
#include "Message_Serializer.h"
#include "Message_Renderer.h"

#ifndef RF24_PRIMARY_PIN_CE
#define RF24_PRIMARY_PIN_CE  9
#endif//RF24_PRIMARY_PIN_CE

#ifndef RF24_PRIMARY_PIN_CSN
#define RF24_PRIMARY_PIN_CSN 10
#endif//RF24_PRIMARY_PIN_CSN

#ifndef RF24_SECONDARY_RADIO_ENABLED
#define RF24_SECONDARY_RADIO_ENABLED  false
#endif//RF24_SECONDARY_RADIO_ENABLED

#if RF24_SECONDARY_RADIO_ENABLED
#ifndef RF24_SECONDARY_PIN_CE
#define RF24_SECONDARY_PIN_CE           A3
#endif//RF24_SECONDARY_PIN_CE

#ifndef RF24_SECONDARY_PIN_CSN
#define RF24_SECONDARY_PIN_CSN          A2
#endif//RF24_SECONDARY_PIN_CSN
#endif//RF24_SECONDARY_RADIO_ENABLED

#define MESSAGE_RENDERERS_LIMIT       7

#ifndef MULTIPLE_RENDERERS_SUPPORTED
#define MULTIPLE_RENDERERS_SUPPORTED  false
#endif//MULTIPLE_RENDERERS_SUPPORTED

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

class RF24Receiver: public MessageProcessor, public TransmissionContext, public TransmissionMonitor {
  public:
    int begin(uint64_t address=RF24_DEFAULT_ADDRESS, void* radio = NULL);
    int check();
    int process(MasterContext* context, JoystickAction* action, MessageInterface* command);
    void reset();
    void set(MessageProcessor* messageProcessor);
    void set(MessageSerializer* messageSerializer);
    void set(MessageRenderer* messageRenderer);
    #if MULTIPLE_RENDERERS_SUPPORTED
    bool add(MessageRenderer* messageRenderer);
    #endif
  protected:
    bool available();
    #if MULTIPLE_RENDERERS_SUPPORTED
    byte invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message,
        MessageInterface* commandPacket, TransmissionCounter* counter);
    #endif
  private:
    void* _receiver = NULL;
    uint32_t _discontinuityCount = 0;
    MessageProcessor* _messageProcessor = NULL;
    MessageSerializer* _messageSerializer = NULL;
    MessageRenderer* _messageRenderer = NULL;
    #if MULTIPLE_RENDERERS_SUPPORTED
    MessageRenderer* _messageRenderers[MESSAGE_RENDERERS_LIMIT] = {};
    uint8_t _messageRenderersTotal = 0;
    #endif
};

class RF24Tranceiver: public RF24Transmitter, public RF24Receiver {
  public:
    RF24Tranceiver(MessageRenderer* messageRenderer = NULL,
        MessageSerializer* _messageSerializer = NULL,
        MessageProcessor* _messageProcessor = NULL);
    int begin(TransmissionProfile* transmissionProfile);
    int begin(tranceiver_t mode=RF24_TX, uint64_t address=RF24_DEFAULT_ADDRESS);
    void reset(tranceiver_t mode=RF24_TX);
  protected:
    void* getPrimaryRadio();
    void* getSecondaryRadio();
  private:
    void* _radio = NULL;
    #if RF24_SECONDARY_RADIO_ENABLED
    void* _radio2nd = NULL;
    #endif
};

#endif
