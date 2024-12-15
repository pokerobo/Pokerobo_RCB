#ifndef __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__
#define __POKEROBO_RCB_PROGRAM_ACCELEROMETER_H__

#include <Pokerobo_RCB.h>
#include "Accelerometer_Handler.h"

class ProgramAccelerometer: public CarCmdProducer {
  public:
    ProgramAccelerometer(char* titleOrTemplate,
      CommandResolver* commandResolver, MessageRenderer* messageRenderer,
      RF24Tranceiver* tranceiver, uint8_t offsetAddress,
      AccelerometerHandler* handler): CarCmdProducer(titleOrTemplate,
        commandResolver, messageRenderer, tranceiver, offsetAddress) {
          this->_accelerometerHandler = handler;
        };
    int check(void* action, void* command=NULL);
    ProgramAccelerometer* setAccelerometerHandler(AccelerometerHandler* handler);
  protected:
    AccelerometerHandler* getAccelerometerHandler();
  private:
    AccelerometerHandler* _accelerometerHandler = NULL;
};

#endif
