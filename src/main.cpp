#include <Arduino.h>
#include <OTA_update.h>

void setup() {
  init_OTA();
  pinMode(D0,OUTPUT);
  digitalWrite(D0, HIGH);
}

void loop() {
  loop_OTA();

}