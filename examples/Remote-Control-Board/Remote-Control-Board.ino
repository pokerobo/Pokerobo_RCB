#include <Pokerobo_RCB_master.h>

#define __RF24_TRANCEIVER_MODE_TX__       1
#define __RF24_TRANCEIVER_MODE_RX__       0

#ifndef __RF24_TRANCEIVER_MODE__
#define __RF24_TRANCEIVER_MODE__          __RF24_TRANCEIVER_MODE_TX__
#endif

const uint64_t address = 0x18580901LL;

DisplayHandler displayHandler;
RF24Tranceiver tranceiver;
JoystickHandler joystickHandler(&tranceiver, &displayHandler);
ConsoleMessageRenderer consoleMessageRenderer;
SpeedResolver speedResolver;

void setup() {
  while (!Serial) delay(100);
  Serial.begin(57600);

  displayHandler.begin();

#if __RF24_TRANCEIVER_MODE__ == __RF24_TRANCEIVER_MODE_TX__
  tranceiver.begin(TX, address);
  joystickHandler.begin();
  joystickHandler.set(&speedResolver);
#else
  tranceiver.begin(RX, address);
  tranceiver.add(&displayHandler);
  tranceiver.add(&consoleMessageRenderer);
#endif
}

void loop() {
#if __RF24_TRANCEIVER_MODE__ == __RF24_TRANCEIVER_MODE_TX__
  int status = joystickHandler.check();
  if (status > 1) {
    delay(10);
  } else {
    delay(5);
  }
#else
  tranceiver.check();
  delay(50);
#endif
}
