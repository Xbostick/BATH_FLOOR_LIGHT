#include <Arduino.h>
#include <OTA_update.h>
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define NUM_LEDS 4
#define PIN D1

bool statuslamp = 0;
int sig = 0;

void setup() {
  init_OTA();
  // CRGB leds[NUM_LEDS];
  // FastLED.addLeds< WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;
  // pinMode(0,INPUT);
 
  // statuslamp=false; // начальное состояние - лампа выключена
}

void loop() {
  // sig = digitalRead(0);
  // //Serial.println (analogRead(A0)); // выводим значение датчика на монитор
  
  //  if(analogRead(A0)>550) {
  //     statuslamp=!statuslamp; // меняем статус лампы при регистрации хлопка
  //     if (statuslamp) FastLED.showColor(CRGB::White);
  //     else FastLED.showColor(CRGB::Blue); 
  //     delay(20); // задержка, "дребезга" хлопко
  //  }



   loop_OTA();

}