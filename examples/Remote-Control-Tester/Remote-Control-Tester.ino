#include <Pokerobo_RCB_quick_start.h>

QuickStartLauncher launcher;

void setup() {
  Serial.begin(57600);
  launcher.begin();
}

void loop() {
  launcher.check();
}
