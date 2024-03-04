#define FASTLED_INTERRUPT_RETRY_COUNT 5
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FastLED.h>
#include "string.h"
#include "pages.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

#include "secure_data.h"

FASTLED_USING_NAMESPACE
#define DEBUG 1
#define NUM_LEDS1 130
#define NUM_LEDS2 200
#define NOISE_PIN A0
#define LED_PIN1 D2
#define LED_PIN2 D3

bool ServerLedState = 0;
bool IS_ON = 0;
bool RDY2USE = 1;
bool SAFEMODE = 1;
bool NEED_REFRESH = 0;
uint8_t count = 0;
uint32_t main_color = 0x00;

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];


int LED = 16; // led connected to D0


ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;

uint32_t str_to_uint32_t(const char* str){
  uint32_t new_uint = 0;
  int ch_2_int = 0;
  for(int i = 1; i < strlen(str); i++){
    // new_uint += int(str[i]);
    ch_2_int = int(str[i]);
    if (ch_2_int < 70) 
      new_uint += (ch_2_int - 48) * pow(16,strlen(str) - i - 1);
    else
      new_uint += (ch_2_int - 87) * pow(16,strlen(str) - i - 1);
  }

  return new_uint;
}

void handle_index(){
// /╲/\[☉﹏☉]/\╱\ <-- Паук! Ааа! 🕷️  

  if (HttpServer.hasArg("change")){
      Serial.print("power changed");
      IS_ON = !IS_ON;
      NEED_REFRESH = true;
    } 

  if (HttpServer.hasArg("plain") and RDY2USE){
    NEED_REFRESH = 1;
    StaticJsonDocument<200> doc;
    deserializeJson(doc, HttpServer.arg("plain"));
    JsonObject object = doc.as<JsonObject>();

    if (object.containsKey("power_change")){
      Serial.print("power changed");
      IS_ON = !IS_ON;
      NEED_REFRESH = true;
    } 
    if (object.containsKey("color")){
      const char* color = object["color"];
      Serial.print("color change to ");
      Serial.println(color);
      main_color = str_to_uint32_t(color);
    } 
  }

  HttpServer.send(200, "text/html", index_page);
  
}

void setup_server(const char* ssid, const char* password){
    Serial.print("Connecting to the Network");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");  
    Serial.println("Server started");

    Serial.print("IP Address of network: "); // Prints IP address on Serial Monitor
    Serial.println(WiFi.localIP());
    Serial.print("Copy and paste the following URL: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    WiFi.begin(ssid, password);

    httpUpdater.setup(&HttpServer, OTAPATH, OTAUSER, OTAPASSWORD);
    HttpServer.on("/", handle_index);
    HttpServer.begin();
}

void server_loop(){
    HttpServer.handleClient();
}

void FastLED_loop(){
  if  (NEED_REFRESH){
    if (IS_ON){
      fill_solid(leds1,NUM_LEDS1,CRGB(main_color));
      fill_solid(leds2,NUM_LEDS2,CRGB(main_color));
      FastLED.show();
     }
    else{
      fill_solid(leds1,NUM_LEDS1,CRGB::Black);
      fill_solid(leds2,NUM_LEDS2,CRGB::Black);
      FastLED.show();
    }
    NEED_REFRESH = 0;
    }
}

bool IsReadyToUse_check_by_EEPROM(){
  EEPROM.begin(8);
  uint8_t restart_num = EEPROM.read(0);
  Serial.print("/nSafemode? Already restarted = ");
  Serial.println(restart_num);
  EEPROM.write(0,restart_num+1);
  EEPROM.commit();
  return restart_num < 3;
}

void DisableSafeMode(){
  EEPROM.write(0,0);
  Serial.println("\nNo safemode now");
  EEPROM.commit();
}

void setup(){ 
  EEPROM.begin(8);
  if (DEBUG)
    Serial.begin(115200);

  pinMode(LED,OUTPUT);

  digitalWrite(LED,0);
  delay(100);
  digitalWrite(LED,1);

  
  RDY2USE = IsReadyToUse_check_by_EEPROM();

  setup_server(ssid,password);

  if (RDY2USE){
    FastLED.addLeds< WS2812B, LED_PIN1, GRB>(leds1, NUM_LEDS1).setCorrection( TypicalLEDStrip );;
    FastLED.addLeds< WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS2).setCorrection( TypicalLEDStrip );;
    FastLED.setMaxPowerInVoltsAndMilliamps(5,5000);
    pinMode(LED_PIN1,OUTPUT);
    pinMode(LED_PIN2,OUTPUT);
  };

  digitalWrite(LED,0);
}

void loop(){

  server_loop();

  if (RDY2USE){
    FastLED_loop();
  }
  
  if ((millis() > 30000) and SAFEMODE) {
    SAFEMODE = false;
    digitalWrite(LED,1);
    delay(500);
    digitalWrite(LED,0);
    DisableSafeMode();
  };
    
}