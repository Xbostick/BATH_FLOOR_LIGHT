#include "primary_functions.h"

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

#ifndef DEBUG
#define DEBUG 0
#endif

bool IsReadyToUse_check_by_EEPROM(){
  EEPROM.begin(8);
  uint8_t restart_num = EEPROM.read(0);
  Serial.print("/nSafemode? Already restarted = ");
  Serial.println(restart_num);
  EEPROM.write(0,restart_num+1);
  EEPROM.commit();
  return ((restart_num < 3) or DEBUG);
}


void DisableSafeMode(){
  EEPROM.write(0,0);
  Serial.println("\nNo safemode now");
  EEPROM.commit();
}

