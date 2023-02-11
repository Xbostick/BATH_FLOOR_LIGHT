#include <Arduino.h>
#include <OTA_update.h>
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define NUM_LEDS 200
#define NOISE_PIN A0
#define LED_PIN D1

bool IS_ON = 0;
float maxv = 0;
int sig = 0;
int noise_time = 0;
int noise_ratio = 0;
int sum;
int count = 0;
float mean = 0;
void setup() {
  //init_OTA();
  Serial.begin(115200);
  CRGB leds[NUM_LEDS];
  FastLED.addLeds< WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;
  pinMode(NOISE_PIN,INPUT);
  pinMode(LED_PIN,OUTPUT);
 
 // начальное состояние - лампа выключена
}

void loop() {
  //loop_OTA();
  //count++;
  noise_ratio = analogRead(NOISE_PIN);
  Serial.println (noise_ratio); // выводим значение датчика на монитор
  FastLED.showColor(CRGB::White);
  delay(1000);
  FastLED.showColor(CRGB::Cyan);
  delay(1000);
  // if (IS_ON && (millis()-noise_time > 60000*60)){
  //   IS_ON = false;
  // }
  // if (IS_ON){
      
  // }
  // else FastLED.showColor(CRGB::Black);

  // if (count >= 100) {
  //   mean = sum/count;
  //   float delta = mean - maxv;
  //   count = 0;
  //   sum = 0;
  //   maxv = 0;
  //   Serial.println(delta);
  //   if((delta < -13)&&(millis() - noise_time > 2000)) {
  //     noise_time = millis();
  //     IS_ON = !IS_ON;
  //     delay(100);
  //  }
  // }
  // else {
  //   if (maxv < noise_ratio){
  //     maxv = noise_ratio;
  //   } 
  //   sum += noise_ratio;
  //   }
   
}