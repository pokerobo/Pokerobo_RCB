#ifndef __RF24_TRANSMITTER_H__
#define __RF24_TRANSMITTER_H__

#include<Arduino.h>

class RF24Tranceiver {
  public:
    int begin();
    bool write(const void* buf, uint8_t len);
};

#endif
