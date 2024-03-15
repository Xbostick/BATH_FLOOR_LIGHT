#ifndef PRIMARY_H
#define PRIMARY_H

#include "string.h"
#include "math.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>

extern HardwareSerial Serial;

uint32_t str_to_uint32_t(const char* str);

bool IsReadyToUse_check_by_EEPROM();

void DisableSafeMode();


#endif