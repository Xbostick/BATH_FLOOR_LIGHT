#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#define OTAUSER         "admin"    // Логин для входа в OTA
#define OTAPASSWORD     "admin"    // Пароль для входа в ОТА
#define OTAPATH         "/"// Путь, который будем дописывать после ip адреса в браузере.
#define SERVERPORT      80         // Порт для входа, он стандартный 80 это порт http 

const char* ssid = "196_kit";
const char* password = "23111970";

IPAddress ip(192,168,0,201);  //статический IP
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;


void init_OTA(){ // ll
    Serial.begin(115200);
    Serial.print("Connecting to the Newtork");
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet);
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
    WiFi.config(ip, gateway, subnet);

    httpUpdater.setup(&HttpServer, OTAPATH, OTAUSER, OTAPASSWORD);
    HttpServer.begin();
    

};

void loop_OTA(){

    HttpServer.handleClient();

}
