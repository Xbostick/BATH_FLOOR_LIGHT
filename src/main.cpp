#define FASTLED_INTERRUPT_RETRY_COUNT 5
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FastLED.h>
#include "string.h"
#include "pages.h"
#include <ArduinoJson.h>
#include "primary_functions.h"
#include "secure_data.h"
#include "websocket.h"
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
char* main_color_str = "#0000ff";

StaticJsonDocument<100> status_json;
const char* status_json_init_string = "{\"power\":\"0\",\"color\":\"#ff0000\"}";

unsigned long ActualCycleTime;

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];

#define DEBUG 0
/* All debug defines here*/
void DistanceSensor_setup();
void DistanceSensor_loop();

void debug_setup(){
  DistanceSensor_setup();
} 

void debug_loop(){
  DistanceSensor_loop();
}
//Тут находится реализация датчика дистанции. Пока что кинут в дебаг, тк на рабочей версии нет датчика)
/*end*/


ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;

WebSocketsServer webSocket = WebSocketsServer(81);

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
      const char* color_str = object["color"];
      strcpy(main_color_str,color_str);
      Serial.print("color change to ");
      Serial.println(main_color_str);
      main_color = str_to_uint32_t(main_color_str);
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
    webSocketBroadcastStatus();
    NEED_REFRESH = 0;
    }
}

void DistanceSensor_setup(){
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
    // Serial.print("Distance: ");
    // Serial.println(distance);

    if (distance < DistanceSensorDistance_sm){
       IS_ON = !IS_ON;
       NEED_REFRESH = 1;
       Serial.print("power change");
       DistanceSensorTimeDelay =  ActualCycleTime;
      }
  }
    
}

void refresh_timers(){
  DistanceSensorTimeTicker = 0;
  DistanceSensorTimeDelay = 0;
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

  if (RDY2USE) {
    webSocket_setup();
  }

  if (DEBUG) debug_setup();

  digitalWrite(LED,0);
}

void loop(){

   if ((count+1) % 2 == 0){
      server_loop();
    }

  if (RDY2USE){
    if (count % 50000 == 0){ // для функций, который не должны крутиться постоянно
      FastLED_loop();
    }
    if (count % 2 == 0){
      webSocket_loop();
    }
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

  count++;
  ActualCycleTime = millis();
}