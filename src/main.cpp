#define FASTLED_INTERRUPT_RETRY_COUNT 5
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>
#include <FastLED.h>
#include "string.h"
#include "pages.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

#include "secure_data.h"

FASTLED_USING_NAMESPACE

#define NUM_LEDS1 130
#define NUM_LEDS2 200
#define LED_PIN1 D2
#define LED_PIN2 D3
#define LED 16 // led connected to D0

#define DistanceSensorTrigPin D5
#define DistanceSensorEchoPin D6
#define DistanceSensorInterval_ms 500
#define DistanceSensorPause_ms 5000
#define DistanceSensorDistance_sm 7
unsigned long DistanceSensorTimeTicker;
unsigned long DistanceSensorTimeDelay;

bool ServerLedState = 0;
bool IS_ON = 0;
bool RDY2USE = 1;
bool SAFEMODE = 1;
bool NEED_REFRESH = 0;

uint8_t count = 0;
uint32_t main_color = 0xFF;
unsigned long ActualCycleTime;

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];

#define DEBUG 0
/* All debug defines here*/

void debug_setup(){
  ;
} 

void debug_loop(){
 ;
}

/*end*/


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

void DistanseSensor_setup(){
  pinMode(DistanceSensorTrigPin, OUTPUT);
  pinMode(DistanceSensorEchoPin, INPUT); 
}

void DistanceSensor_loop(){
  if ((ActualCycleTime - DistanceSensorTimeTicker > DistanceSensorInterval_ms) 
  and (ActualCycleTime - DistanceSensorTimeDelay  > DistanceSensorPause_ms)){
    DistanceSensorTimeTicker = ActualCycleTime;
    digitalWrite(DistanceSensorTrigPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(DistanceSensorTrigPin, LOW);

    float distance = ( pulseIn(DistanceSensorEchoPin, HIGH)*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < DistanceSensorDistance_sm){
       IS_ON = !IS_ON;
       NEED_REFRESH = 1;
       Serial.print("power change");
       DistanceSensorTimeDelay =  ActualCycleTime;
      }
  }
    
}


bool IsReadyToUse_check_by_EEPROM(){
  EEPROM.begin(8);
  uint8_t restart_num = EEPROM.read(0);
  Serial.print("/nSafemode? Already restarted = ");
  Serial.println(restart_num);
  EEPROM.write(0,restart_num+1);
  EEPROM.commit();
  return ((restart_num < 3) or DEBUG);
}

void refresh_timers(){
  DistanceSensorTimeTicker = 0;
  DistanceSensorTimeDelay = 0;
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

  if (RDY2USE) DistanseSensor_setup();
  if (DEBUG) debug_setup();

  digitalWrite(LED,0);
}

void loop(){

  server_loop();

  if (RDY2USE){
    FastLED_loop();
    DistanceSensor_loop();
  }

  if ((ActualCycleTime > 30000) and SAFEMODE) {
    SAFEMODE = false;
    digitalWrite(LED,1);
    delay(500);
    digitalWrite(LED,0);
    DisableSafeMode();
  };
    
  if (DEBUG) debug_loop();

  if (ActualCycleTime > millis())
  {
    refresh_timers();
  }

  ActualCycleTime = millis();
}