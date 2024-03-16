#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

extern WebSocketsServer webSocket;
extern bool IS_ON;
extern char* main_color_str;

extern StaticJsonDocument<100> status_json;
extern const char* status_json_init_string;


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void webSocket_setup();
void webSocket_loop();
void webSocketBroadcastStatus();

#endif