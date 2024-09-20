#ifndef __POKEROBO_RCB_CONFIGURATION_H__
#define __POKEROBO_RCB_CONFIGURATION_H__

#include "Commons.h"

struct Configuration {
  public:
    static Configuration* me;
    Configuration(
      uint8_t pinOfJoystickX_,
      uint8_t pinOfJoystickY_,
      uint8_t pinOfUpButton_,
      uint8_t pinOfRightButton_,
      uint8_t pinOfDownButton_,
      uint8_t pinOfLeftButton_,
      uint8_t pinOfStartButton_,
      uint8_t pinOfSelectButton_,
      uint8_t pinOfAnalogButton_,
      uint16_t joystickHighLevelPins_,
      uint16_t joystickDisabledButtons_,
      uint8_t rf24PrimaryPinCE_,
      uint8_t rf24PrimaryPinCSN_,
      uint16_t rf24ReceiverDiscontinuityMax_,
      bool rf24DuplexInteractionEnabled_,
      bool wireWithTimeout_
    ):
      pinOfJoystickX(pinOfJoystickX_),
      pinOfJoystickY(pinOfJoystickY_),
      pinOfUpButton(pinOfUpButton_),
      pinOfRightButton(pinOfRightButton_),
      pinOfDownButton(pinOfDownButton_),
      pinOfLeftButton(pinOfLeftButton_),
      pinOfStartButton(pinOfStartButton_),
      pinOfSelectButton(pinOfSelectButton_),
      pinOfAnalogButton(pinOfAnalogButton_),
      joystickHighLevelPins(joystickHighLevelPins_),
      joystickDisabledButtons(joystickDisabledButtons_),
      rf24PrimaryPinCE(rf24PrimaryPinCE_),
      rf24PrimaryPinCSN(rf24PrimaryPinCSN_),
      rf24ReceiverDiscontinuityMax(rf24ReceiverDiscontinuityMax_),
      rf24DuplexInteractionEnabled(rf24DuplexInteractionEnabled_),
      wireWithTimeout(wireWithTimeout_)
    {
      me = this;
    }
    const uint8_t pinOfJoystickX;
    const uint8_t pinOfJoystickY;
    const uint8_t pinOfUpButton;
    const uint8_t pinOfRightButton;
    const uint8_t pinOfDownButton;
    const uint8_t pinOfLeftButton;
    const uint8_t pinOfStartButton;
    const uint8_t pinOfSelectButton;
    const uint8_t pinOfAnalogButton;
    const uint16_t joystickHighLevelPins;
    const uint16_t joystickDisabledButtons;
    const uint8_t rf24PrimaryPinCE;
    const uint8_t rf24PrimaryPinCSN;
    const uint16_t rf24ReceiverDiscontinuityMax;
    const bool rf24DuplexInteractionEnabled;
    const bool wireWithTimeout;
};

#endif
