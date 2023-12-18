#include "Message_Renderer.h"

void ConsoleMessageRenderer::clear() {}

void ConsoleMessageRenderer::splash(char* title, byte align) {}

void ConsoleMessageRenderer::render(JoystickAction* message, MessageInterface* commandPacket, TransmissionCounter* counter) {
  MovingCommand* movingCommand = (MovingCommand*) commandPacket;
  Serial.print('#'), Serial.print(message->getExtras()), Serial.print(' '), Serial.print('-'), Serial.print(' ');
  Serial.print("Pressing"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getPressingFlags());
  Serial.print(';'), Serial.print(' ');
  Serial.print("Clicking"), Serial.print("Flags"), Serial.print(':'), Serial.print(' '),
      Serial.print(message->getClickingFlags());
  Serial.print(';'), Serial.print(' '), Serial.print('X'), Serial.print(':'), Serial.print(' '), Serial.print(message->getX());
  Serial.print(';'), Serial.print(' '), Serial.print('Y'), Serial.print(':'), Serial.print(' '), Serial.print(message->getY());
  Serial.println();
}
