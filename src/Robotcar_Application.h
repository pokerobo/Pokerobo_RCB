#ifndef __POKEROBO_RCB_ROBOTCAR_APPLICATION_H__
#define __POKEROBO_RCB_ROBOTCAR_APPLICATION_H__

#include "Commons.h"
#include "Display_Handler.h"
#include "Joystick_Action.h"
#include "Message_Exchange.h"
#include "Message_Serializer.h"
#include "Message_Renderer.h"
#include "Message_Resolver.h"

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
    CommandPacket* create();
    CommandPacket* resolve(CommandPacket* command, JoystickAction* action);
    void release(CommandPacket* command);
  private:
    int _coeff = 3;
    bool _rotatable = false;
    bool _reversed = false;
};

class MovingMessageSerializer: public MessageSerializer {
  public:
    uint8_t getSize();
    int decode(uint8_t* msg, MessageProcessor *processor);
  private:
    static const uint8_t messageSize;
};

class MovingDisplayHandler: public DisplayHandler {
  protected:
    void renderCommandPacket_(uint8_t lx, uint8_t ty, MessageInterface* commandPacket);
};

class MovingSerialConsole: public MessageRenderer {
  public:
    void clear();
    void splash(char* title, byte align = 0);
    void render(JoystickAction* message, MessageInterface* commandPacket=NULL, TransmissionCounter* counter=NULL);
};

#endif
