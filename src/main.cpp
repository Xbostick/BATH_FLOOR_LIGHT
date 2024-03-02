// #include <Arduino.h>
// #include <OTA_update.h>
// #include <FastLED.h>

// #include <ESP8266WiFi.h>
// #include "index.h"
// #include <ESP8266WebServer.h>
// #include <ESP8266HTTPUpdateServer.h>

// #define OTAUSER         "admin"    // Логин для входа в OTA
// #define OTAPASSWORD     "admin"    // Пароль для входа в ОТА
// #define OTAPATH         "/UPDATE"// Путь, который будем дописывать после ip адреса в браузере.
// #define SERVERPORT      80         // Порт для входа, он стандартный 80 это порт http 

// const char* ssid = "196_kit";
// const char* password = "23111970";

// IPAddress ip(192,168,0,201);  //статический IP
// IPAddress gateway(192,168,0,1);
// IPAddress subnet(255,255,255,0);

// ESP8266WebServer HttpServer(SERVERPORT);
// ESP8266HTTPUpdateServer httpUpdater;

// WiFiServer server(80);

// FASTLED_USING_NAMESPACE
// #define NUM_LEDS 250
// #define NOISE_PIN A0
// #define LED_PIN1 D1
// #define LED_PIN2 D2
// #define LED_PIN3 D3

// bool IS_ON = 0;
// float maxv = 0;
// int sig = 0;
// int noise_time = 0;
// int noise_ratio = 0;
// int sum;
// int count = 0;
// float mean = 0;
// int LED = 16; // led connected to D0
// ESP8266HTTPUpdateServer httpUpdater;
// void setup() {

//   init_OTA();
//   Serial.begin(115200);
//   CRGB leds1[NUM_LEDS];
//   CRGB leds2[NUM_LEDS];
//   CRGB leds3[NUM_LEDS];
//   FastLED.addLeds< WS2812B, LED_PIN1, GRB>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );;
//   FastLED.addLeds< WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );;
//   FastLED.addLeds< WS2812B, LED_PIN3, GRB>(leds3, NUM_LEDS).setCorrection( TypicalLEDStrip );;
//   pinMode(NOISE_PIN,INPUT);
//   pinMode(LED_PIN1,OUTPUT);
//   pinMode(LED_PIN2,OUTPUT);
//   pinMode(LED_PIN3,OUTPUT);
//   pinMode(LED,OUTPUT);
 
  
//   FastLED.showColor(CRGB::Cyan);
//   const char* ssid = "196";
// const char* password = "23111970";




// Serial.print("\nConnecting to the Newtork");
// httpUpdater.setup(&WiFi, OTAPATH, OTAUSER, OTAPASSWORD);
// WiFi.begin(ssid, password);
// //WiFi.config(ip, gateway, subnet);
// while (WiFi.status() != WL_CONNECTED)
// {
//   delay(500);
//   Serial.print(".");
// }
// Serial.println("WiFi connected");  
// server.begin();  // Starts the Server
// Serial.println("Server started");

// Serial.print("IP Address of network: "); // Prints IP address on Serial Monitor
// Serial.println(WiFi.localIP());
// Serial.print("Copy and paste the following URL: http://");
// Serial.print(WiFi.localIP());
// Serial.println("/");
//  // начальное состояние - лампа выключена
// }

// void loop() {
//   loop_OTA();
//   //count++;
//   //noise_ratio = analogRead(NOISE_PIN);
//   //Serial.println (noise_ratio); // выводим значение датчика на монитор
//   count++;
//   if( count % 10 == 0)
//   {WiFiClient client = server.available();
//   if (!client)
//   {
//     return;
//   }
  
//   while(!client.available())
//   {
//     delay(1);
//   }

//   String request = client.readStringUntil('\r');
//   Serial.println(request);
//   client.flush();

//   int value = LOW;
// /*------------------Requests logic---------------------*/

//   if(request.indexOf("/LED=CHANGE") != -1)
//   {
//     if (IS_ON)
//     IS_ON=0;
//     else
//     IS_ON=1;
//     Serial.println("on LED");
//   }
//   if(request.indexOf("/LED=OFF") != -1)
//   {
//      IS_ON = 0;
//     Serial.println("off LED");;
//   }

// /*------------------HTML Page Creation---------------------*/
//   index_html(client);}
//   if  (IS_ON)  {
//     FastLED.showColor(CRGB::Cyan);
//     digitalWrite(LED,1);
//     }
//   else {
//     FastLED.showColor(CRGB::Black);
//     digitalWrite(LED,0);}
//   // if (IS_ON && (millis()-noise_time > 60000*60)){
//   //   IS_ON = false;
//   // }
//   // if (IS_ON){
      
//   // }
//   // else FastLED.showColor(CRGB::Black);

//   // if (count >= 100) {
//   //   mean = sum/count;
//   //   float delta = mean - maxv;
//   //   count = 0;
//   //   sum = 0;
//   //   maxv = 0;
//   //   Serial.println(delta);
//   //   if((delta < -13)&&(millis() - noise_time > 2000)) {
//   //     noise_time = millis();
//   //     IS_ON = !IS_ON;
//   //     delay(100);
//   //  }
//   // }
//   // else {
//   //   if (maxv < noise_ratio){
//   //     maxv = noise_ratio;
//   //   } 
//   //   sum += noise_ratio;
//   //   }
   
// }

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FastLED.h>
#include "string.h"
#include "pages.h"

FASTLED_USING_NAMESPACE
#define NUM_LEDS 100
#define NOISE_PIN A0
#define LED_PIN1 D2
#define LED_PIN2 D3

bool ServerCmdChange = 0;
bool IS_ON = 0;

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];


int LED = 16; // led connected to D0
#define OTAUSER         "admin"    // Логин для входа в OTA
#define OTAPASSWORD     "231170"    // Пароль для входа в ОТА
#define OTAPATH         "/UPDATE"// Путь, который будем дописывать после ip адреса в браузере.
#define SERVERPORT      80         // Порт для входа, он стандартный 80 это порт http 

const char* ssid = "196_kit";
const char* password = "23111970";

ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;

void handle_switcher(){

    if (ServerCmdChange){
    ServerCmdChange=0;
    Serial.println("on new LED");}
    else{
    ServerCmdChange=1;
    Serial.println("off new LED");
    }
    HttpServer.send(200, "text/html", temp);
}

void handle_index(){
// /╲/\[☉﹏☉]/\╱\ <-- Паук! Ааа! 🕷️  
  HttpServer.send(200, "text/html", temp);
}


void setup_server(const char* ssid, const char* password){
    Serial.begin(115200);
    Serial.print("Connecting to the Newtork");
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
    Serial.print("Copy and paste the following URL: https://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    WiFi.begin(ssid, password);

    httpUpdater.setup(&HttpServer, OTAPATH, OTAUSER, OTAPASSWORD);
    HttpServer.on("/", handle_index);
    HttpServer.on("/LED=SWITCH", handle_switcher);
    HttpServer.begin();
}

void server_loop(){
    HttpServer.handleClient();
}

void setup(){
  setup_server(ssid,password);

  FastLED.addLeds< WS2812B, LED_PIN1, GRB>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );;
  FastLED.addLeds< WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );;
  FastLED.setMaxPowerInVoltsAndMilliamps(5,5000);
  pinMode(NOISE_PIN,INPUT);
  pinMode(LED_PIN1,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  pinMode(LED,OUTPUT); 
  fill_solid(leds1,NUM_LEDS,CRGB::HotPink);
  FastLED.show();
  delay(10);
}

void loop(){
  server_loop();
  // if  (ServerCmdChange != IS_ON)  {
  //   if (IS_ON){
  //     for (int i = 0; i < NUM_LEDS; i++){
  //       leds1[i] = CRGB::HotPink;
  //       leds2[i] = 
  //     }
  //     FastLED.show();
  //     digitalWrite(LED,1);
  //   }
  //   else{
  //     FastLED.clear();
  //     FastLED.show();
  //     digitalWrite(LED,0);
  //   }
  //   IS_ON = !IS_ON;
  //   }
  if (ServerCmdChange) FastLED.showColor(CRGB::HotPink);
  else FastLED.clear();

  
    
}