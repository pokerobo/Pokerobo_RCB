#include "Message_Exchange.h"
#include "Message_Serializer.h"
#include "Moving_Command.h"

int MovingMessageSerializer::decode(uint8_t* msg, MessageProcessor* processor) {
  uint16_t buttons;
  uint16_t jX, jY;
  uint32_t count;

  bool ok = decodeMessage(msg, MESSAGE_SIGNATURE, &buttons, &jX, &jY, &count);

  if (!ok) {
    return -1;
  }

  #if __DEBUG_LOG_RF24_TRANCEIVER__
  char log[32] = { 0 };
  buildJoystickActionLogStr(log, buttons, jX, jY, count);
  Serial.print("decode"), Serial.print('('), Serial.print(log), Serial.print(')'),
      Serial.print(' '), Serial.print('-'), Serial.print('>'), Serial.print(' '), Serial.print(ok);
  Serial.println();
  #endif

  JoystickAction message(buttons, jX, jY, count);

  MovingCommand movingCommandInstance;
  MessageInterface* commandPacket = movingCommandInstance.deserialize(
      msg + strlen(MESSAGE_SIGNATURE) + JoystickAction::messageSize);

  if (processor != NULL) {
    return processor->process(&message, commandPacket);
  }

  return 0;
}
