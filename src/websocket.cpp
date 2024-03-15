#include "websocket.h"


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        const uint8_t size = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<size> json;
        // Send the current LED status
        if (IS_ON) {
          json["status"] = "+";
        }
        else {
          json["status"] = "-";
        }
        String data;
        serializeJson(json, data);
        webSocket.sendTXT(num,data);
      }        
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}

void webSocket_setup(){
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void webSocket_loop(){
    webSocket.loop();
}

void webSocketBroadcastStatus(){
        const uint8_t size = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<size> json;
        // Send the current LED status
        if (IS_ON) {
          json["status"] = "+";
        }
        else {
          json["status"] = "-";
        }
        String data;
        serializeJson(json, data);
        webSocket.broadcastTXT(data);
    
}