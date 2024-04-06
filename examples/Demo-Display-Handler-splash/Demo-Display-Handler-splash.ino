#include <Pokerobo_RCB_master.h>

DisplayHandler displayHandler;
int count = 0;

void setup() {
  Serial.begin(57600);
  displayHandler.begin();
}

void loop() {
  char[20] buf = {};
  count += 1;
  displayHandler.splash(itoa(count, buf, 10));
}
