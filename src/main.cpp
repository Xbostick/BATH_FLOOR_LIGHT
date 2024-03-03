#define FASTLED_INTERRUPT_RETRY_COUNT 5
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FastLED.h>
#include "string.h"
#include "pages.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

FASTLED_USING_NAMESPACE
#define NUM_LEDS1 130
#define NUM_LEDS2 200
#define NOISE_PIN A0
#define LED_PIN1 D2
#define LED_PIN2 D3

bool ServerCmdChange = 0;
bool IS_ON = 0;
bool RDY2USE = 1;
bool SAFEMODE = 1;
uint8_t count = 0;

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];


int LED = 16; // led connected to D0
#define OTAUSER         "admin"    // Логин для входа в OTA
#define OTAPASSWORD     "231170"    // Пароль для входа в ОТА
#define OTAPATH         "/UPDATE"// Путь, который будем дописывать после ip адреса в браузере.
#define SERVERPORT      80         // Порт для входа, он стандартный 80 это порт http 

const char* ssid = "196_kit";
const char* password = "23111970";

ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;

void handle_index(){
// /╲/\[☉﹏☉]/\╱\ <-- Паук! Ааа! 🕷️  
  Serial.print("comming\n");
  String message;
  for (int i = 0; i < HttpServer.args(); i++) {
        message += "Arg nº" + (String)i + " –> ";
        message += HttpServer.argName(i) + ": ";
        message += HttpServer.arg(i) + "\n";
        } 
 
  Serial.println(HttpServer.arg("plain"));

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, HttpServer.arg("plain"));
  Serial.println(doc["color"].as<String>());
  Serial.println(doc.as<String>());
  
  HttpServer.send(200, "text/html", index_page);
}

void handle_json(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, HttpServer.arg('color'));
  Serial.println(doc["color"].as<String>());

}

void handle_switcher(){

    if (ServerCmdChange){
    ServerCmdChange=0;
    Serial.println("on new LED");}
    else{
    ServerCmdChange=1;
    Serial.println("off new LED");
    }
    handle_index();
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
    if (RDY2USE){
      HttpServer.on("/LED=SWITCH", handle_switcher);
      HttpServer.on("/", HTTP_POST, handle_json);}

    HttpServer.begin();
}

void server_loop(){
    HttpServer.handleClient();
}

void FastLED_loop(){
  if  (ServerCmdChange != IS_ON)  {
    if (IS_ON){
      fill_solid(leds1,NUM_LEDS1,CRGB::White);
      fill_solid(leds2,NUM_LEDS2,CRGB::White);
      FastLED.show();
     }
    else{
      fill_solid(leds1,NUM_LEDS1,CRGB::Black);
      fill_solid(leds2,NUM_LEDS2,CRGB::Black);
      FastLED.show();
    }
    IS_ON = ServerCmdChange;
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