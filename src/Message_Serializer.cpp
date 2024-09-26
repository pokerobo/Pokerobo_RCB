#include "Message_Serializer.h"

const uint8_t JoystickMessageSerializer::messageSize = MESSAGE_SIGNATURE_SIZE +
    JoystickAction::messageSize;

uint8_t JoystickMessageSerializer::getSize() {
  return messageSize;
}

int JoystickMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  if (msg[0] == 'J' && msg[1] == 'S') {
    JoystickAction controlAction;
    if (controlAction.deserialize(msg + MESSAGE_SIGNATURE_SIZE) == NULL) {
      return -1;
    }
    JoystickAction* action = &controlAction;

    if (processor != NULL) {
      return processor->process(NULL, action, NULL);
    }

    return 0;
  }

  uint8_t offset = 0;

  MasterContext contextPacket;
  if (contextPacket.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += contextPacket.length();
  MasterContext* context = &contextPacket;

  JoystickAction controlAction;
  if (controlAction.deserialize(msg + offset) == NULL) {
    return -1;
  }
  offset += controlAction.length();
  JoystickAction* action = &controlAction;

  if (processor != NULL) {
    return processor->process(context, action, NULL);
  }

  return 0;
}
