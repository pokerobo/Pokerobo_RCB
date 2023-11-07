#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include "Commons.h"
#include "Message_Sender.h"
#include "Speed_Resolver.h"

#define RF24_DEFAULT_ADDRESS 0x123456789ABCDEF0LL

#define MESSAGE_RENDERERS_LIMIT   7
#define MULTIPLE_RENDERERS_SUPPORTED true

typedef enum { TX = 0, RX } tranceiver_t;
typedef enum { ACK_OK = 0, ACK_FAILED, MESSAGE_NULL, TRANSMITTER_NULL } rf24_tx_status_t;

class RF24Transmitter: public MessageSender {
  public:
    int begin(void* radio, uint64_t address=RF24_DEFAULT_ADDRESS);
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
    int begin(void* radio, uint64_t address=RF24_DEFAULT_ADDRESS);
    int check();
    void set(MessageRenderer* messageRenderer);
#if MULTIPLE_RENDERERS_SUPPORTED
    bool add(MessageRenderer* messageRenderer);
#endif
    void set(SpeedResolver* speedResolver);
    void reset();
  protected:
#if MULTIPLE_RENDERERS_SUPPORTED
    byte invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message);
#endif
  private:
    void* _receiver = NULL;
    MessageRenderer* _messageRenderer = NULL;
#if MULTIPLE_RENDERERS_SUPPORTED
    MessageRenderer* _messageRenderers[MESSAGE_RENDERERS_LIMIT] = {};
    uint8_t _messageRenderersTotal = 0;
#endif
    SpeedResolver* _speedResolver = NULL;
};

class RF24Tranceiver: public RF24Transmitter, public RF24Receiver {
  public:
    int begin(tranceiver_t mode=TX, uint64_t address=RF24_DEFAULT_ADDRESS);
    void reset(tranceiver_t mode=TX);
};

#endif
