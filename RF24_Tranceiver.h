#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include "Commons.h"
#include "Message_Sender.h"

typedef enum { TX = 0, RX } tranceiver_t;

class RF24Transmitter: public MessageSender {
  public:
    int begin(void* radio);
    bool write(const void* buf, uint8_t len);
  private:
    void* _transmitter = NULL;
};

class RF24Receiver {
  public:
    int begin(void* radio);
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
    int begin(tranceiver_t mode=TX);
};

#endif
