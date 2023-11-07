#ifndef __PROGRAM_SELECTOR_H__
#define __PROGRAM_SELECTOR_H__

#include "Commons.h"
#include "Joystick_Handler.h"
#include "RF24_Tranceiver.h"

#define PROGRAM_MODE_PLAYER                 5
#define PROGRAM_MODE_TESTER                 9

#define PROGRAM_NRF24_REAL_TRANSMITTER      1
#define PROGRAM_NRF24_REAL_RECEIVER         2
#define PROGRAM_NRF24_TEST_TRANSMITTER      4
#define PROGRAM_NRF24_TEST_RECEIVER         8

class ProgramSelector {
  public:
    int begin(uint8_t mode=PROGRAM_MODE_PLAYER);
    void set(MessageRenderer* messageRenderer);
    void set(JoystickHandler* joystickHandler);
    void set(RF24Tranceiver* tranceiver, uint64_t address);
    int check();
  private:
    uint8_t _mode = PROGRAM_MODE_PLAYER;
    MessageRenderer* _messageRenderer = NULL;
    JoystickHandler* _joystickHandler = NULL;
    RF24Tranceiver* _rf24Tranceiver = NULL;
    uint64_t _rf24Address = RF24_DEFAULT_ADDRESS;
    uint8_t _currentState = PROGRAM_NRF24_REAL_TRANSMITTER;
    int move_();
};

#endif
