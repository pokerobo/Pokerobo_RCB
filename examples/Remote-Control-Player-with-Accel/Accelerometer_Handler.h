#ifndef __POKEROBO_RCB_ACCELEROMETER_HANDLER_H__
#define __POKEROBO_RCB_ACCELEROMETER_HANDLER_H__

#include <Arduino.h>
#include <Wire.h>

class AccelerometerHandler {
  public:
    virtual void begin();
    virtual void read();
    virtual float getRoll();
    virtual float getPitch();
};

#endif
