#include "globals.h"

void setup(void) {
  #ifdef DEBUG
    Serial.begin(9600); 
    Serial.println("");
  #endif
  eeprom_begin();
  eeprom_load();
  //eeprom_save();
  display_start();
  display_screen_welcome();
  delay(lcd_splash_time * 1000);
  wifi_start();
  ethernet_start();
  artnet_start();
  display_data_update();
  #ifndef DEBUG
    dmxA.begin();
    dmxA.clearChans();
    dmxDataA = dmxA.getChans();
  #endif
  dmxB.begin();
  dmxB.clearChans();
  dmxDataB = dmxB.getChans();
  encoder_start();
  interrupts_start();
  }

void loop(void) {
  //yield();
  display_screen_main();
  encoder_service();  
  }

