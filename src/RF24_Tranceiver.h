#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include "Commons.h"
#include "Message_Sender.h"

#define RF24_DEFAULT_ADDRESS 0x123456789ABCDEF0LL

typedef enum { TX = 0, RX } tranceiver_t;
typedef enum { ACK_OK = 0, ACK_FAILED, MESSAGE_NULL, TRANSMITTER_NULL } rf24_tx_status_t;

class RF24Transmitter: public MessageSender {
  public:
    int begin(void* radio, uint64_t address);
    bool write(const void* buf, uint8_t len);
    bool write(MessagePacket* packet);
    rf24_tx_status_t getStatus();
  private:
    void* _transmitter = NULL;
    rf24_tx_status_t _status;
};

class RF24Receiver {
  public:
    int begin(void* radio, uint64_t address);
    int check();
    bool add(MessageRenderer* messageRenderer);
  protected:
    byte invoke(MessageRenderer* messageRenderer, uint8_t index, JoystickAction* message);
  private:
    void* _receiver = NULL;
    MessageRenderer* _messageRenderers[MESSAGE_RENDERERS_LIMIT] = {};
    uint8_t _messageRenderersTotal = 0;
};

class RF24Tranceiver: public RF24Transmitter, public RF24Receiver {
  public:
    int begin(tranceiver_t mode=TX, uint64_t address=RF24_DEFAULT_ADDRESS);
};

#endif