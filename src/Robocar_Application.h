#ifndef __POKEROBO_RCB_ROBOCAR_APPLICATION_H__
#define __POKEROBO_RCB_ROBOCAR_APPLICATION_H__

#include "Commons.h"
#include "Joystick_Action.h"
#include "Display_X_Handler.h"
#include "Message_Exchange.h"
#include "Message_Serializer.h"
#include "Message_Renderer.h"
#include "Message_Resolver.h"

#ifndef MOVING_COMMAND_BOUND_X
#define MOVING_COMMAND_BOUND_X                40
#endif//MOVING_COMMAND_BOUND_X

#ifndef MOVING_COMMAND_BOUND_Y
#define MOVING_COMMAND_BOUND_Y                40
#endif//MOVING_COMMAND_BOUND_Y

#define MOVING_COMMAND_WEIGHT_MAX             255

class MovingCommandPacket: public CommandPacket {
  public:
    static const uint8_t messageSize;
    MovingCommandPacket(int leftSpeed=0, byte leftDirection=0, int rightSpeed=0, byte rightDirection=0);
    void update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection);
    void update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection, bool reversed);
    int getLeftSpeed();
    byte getLeftDirection();
    int getRightSpeed();
    byte getRightDirection();
    bool isReversed();
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
    CommandPacket* deserialize(uint8_t* buf);
  private:
    int _LeftSpeed;
    byte _LeftDirection;
    int _RightSpeed;
    byte _RightDirection;
    bool _reversed = false;
};

class MovingCommandResolver: public CommandResolver {
  public:
    MovingCommandResolver(bool reversed=false);
    bool isReversed();
    void setReversed(bool reversed);
    int getCoefficient();
    void setCoefficient(int coefficient);
    CommandPacket* create();
    virtual CommandPacket* resolve(CommandPacket* command, JoystickAction* action);
    void release(CommandPacket* command);
  protected:
    virtual void onForward(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onForwardLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onForwardRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onBackwardRight(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onBackwardLeft(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
    virtual void onBackward(int x, int y, int &leftSpeed, byte &leftDirection, int &rightSpeed, byte &rightDirection);
  private:
    int _coefficient = 3;
    bool _reversed = false;
};

class CtrlCarMessageSerializer: public MessageSerializer {
  public:
    virtual uint8_t getSize();
    virtual int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};

#if __LEGACY_MESSAGE_SERIALIZER__
class MovingMessageSerializer: public MessageSerializer {
  public:
    uint8_t getSize();
    int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};
#else
class MovingMessageSerializer: public CtrlCarMessageSerializer {
  public:
    uint8_t getSize();
    int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};
#endif

class MovingDisplayHandler: public DeviceDisplayHandler {
  protected:
    void renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket);
};

class MovingSerialConsole: public MessageRenderer {
  public:
    void clear();
    void notify(char* title, byte align = 0);
    void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
};

#endif
