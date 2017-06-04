void display_start(){
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setContrast(lcd_contrast);
  }

void display_data_update(){
  int current_time = millis();
  //common
  lcd_uptime = current_time/1000;
  //Serial.println(lcd_uptime);
  //Wifi
  lcd_wifi_rssi = constrain((WiFi.RSSI()+100)/1.3, 0, 70); //0 = good, -100 = bad
  if (wifi_radio){
    if (wifi_mode_ap) {char ssidChar[30]; Wifi_get_ap_name().toCharArray(lcd_wifi_ssid, 30);}
      else {strncpy(lcd_wifi_ssid, wifi_client_ssid, 30);}
    if (wifi_mode_ap){ lcd_wifi_mode = "AP"; }
      else {
        if (WiFi.status() == WL_CONNECTED){lcd_wifi_mode = "CL";}
          else{lcd_wifi_mode = "ERR";}
        }
    }
  else {
     lcd_wifi_mode = "OFF";
     strncpy(lcd_wifi_ssid, "WiFi disabled", 30);
     }
  lcd_artnet_time_a = (current_time - artNetUniA_LastReceived)/1000;
  lcd_artnet_time_b = (current_time - artNetUniB_LastReceived)/1000;
  lcd_artnet_time_last_artdmx = (current_time - artNet_LastReceived_Time)/1000;
  }

/*
Graphics
*/
void display_hLine(uint8_t x, uint8_t y, uint8_t length){
  u8g2.drawLine(x, y, x+length, y);
  }

void display_vLine(uint8_t x, uint8_t y, uint8_t height){
  u8g2.drawLine(x, y, x, y+height);
  }

/*
Display Messages
*/
void display_item_last_artnet_data(uint8_t x, uint8_t y, int val){
  if (val == 0) {
      u8g2.setFont(u8g2_font_m2icon_9_tf);
      u8g2.drawStr(x+0, y+7, "\x44");
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(x+12, y+7, "Receiving");
      }
  else {
      u8g2.setFont(u8g2_font_m2icon_9_tf);
      u8g2.drawStr(x+2, y+5, "\x43");
      display_text_time_16x18(x+12, y, val);
      }
  }

void display_item_artnet_addr(uint8_t x, uint8_t y){
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(x+0, y+7, "Net:");
  u8g2.drawStr(x+46, y+7, "Sub:");
  u8g2.setFont(u8g2_font_ncenB10_tr);
  display_text_int(x+20, y+7, artNetSub);
  display_text_int(x+66, y+7, artNetNet);
  }

void display_item_artnet_uni(uint8_t x, uint8_t y, uint8_t val){
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(x+0, y+7, "Uni:");
  u8g2.setFont(u8g2_font_ncenB10_tr);
  display_text_int(x+20, y+7, val);
  }

/*
Display Screens
*/
void display_screen_main(){
  uint8_t x = 0;
  uint8_t y = 0;
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.drawBox(0, 0, lcd_wifi_rssi/3.5, 8);
  u8g2.setFont(u8g2_font_5x7_tf);
  display_text_str(23, 7, lcd_wifi_ssid, 18);
  display_text_str(113, 7, lcd_wifi_mode, 0);
  display_hLine(0, 8, 128);
  display_vLine(20, 0, 8);
  display_vLine(110, 0, 8);
  yield();
  //MAIN
  u8g2.setFont(u8g2_font_5x7_tf);
  display_text_int(0, 16, menu);
  /*Status*/
  if (menu == MENU_STATUS){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Status"); 
      display_item_artnet_addr(45,15);
      display_item_artnet_uni(4, 31, artNetUniA); 
      display_item_artnet_uni(4, 46, artNetUniB); 
      display_item_last_artnet_data(43, 31, lcd_artnet_time_a);
      display_item_last_artnet_data(43, 46, lcd_artnet_time_b);
      }
  /*Last artDMX received*/
  if (menu == MENU_LAST_ARDDMX){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Last ArtDMX Received"); 
      //last pack
      x = 4; y = 31;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+0, y+7, "Net:");
      u8g2.drawStr(x+46, y+7, "Sub:");
      u8g2.drawStr(x+84, y+7, "Uni:");
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+20, y+7, artNet_LastReceived_Net);     
      display_text_int(x+66, y+7, artNet_LastReceived_SubNet);
      display_text_int(x+103, y+7, artNet_LastReceived_Uni);
      //last time
      x = 4; y = 46;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x-1, y+7, "Time:");
      if (lcd_artnet_time_last_artdmx == 0) {
          u8g2.setFont(u8g2_font_ncenB10_tr);
          u8g2.drawStr(x+25, y+7, "Receiving");
          }
      else {display_text_time_16x18(x+24, y, lcd_artnet_time_last_artdmx);}  
      }
  /*DMX viewer*/
  if (menu == MENU_DMXVIEW){
      uint8_t val;  
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "DMX Channel Viewer"); 
      //channel display
      x = 3; y = 17;
        u8g2.setFont(u8g2_font_5x7_tf);
        //channel overview
        if (dmxviewer_channel == 0){
          display_text_int(x+0,y+31,(dmxviewer_page*20)+1);
          display_text_int(x+105,y+31,(dmxviewer_page*20)+20);
          for (int i = 0; i < 20; i++){
            uint16_t chan = (dmxviewer_page*20)+i;
            if (chan > 511)break;         
            u8g2.drawBox(x+(i*6), y, 5, 24);
              #ifndef DEBUG
                if (dmxviewer_source == 0){val = dmxDataA[chan];}
              #endif
              if (dmxviewer_source == 1){val = dmxDataB[chan];}
              uint8_t value_draw = 22-(val/11.6);
              if (value_draw != 0){
                u8g2.setDrawColor(0);
                u8g2.drawBox(x+(i*6)+1, y+1, 3, value_draw);
                u8g2.setDrawColor(1);
                }
            }
          }
        //channel detail
        else {
          x = 0; y = 19;
          #ifndef DEBUG
            if (dmxviewer_source == 0){val = dmxDataA[dmxviewer_channel-1];}
          #endif
          if (dmxviewer_source == 1){val = dmxDataB[dmxviewer_channel-1];}
          dmxviewer_history[dmxviewer_history_counter++] = val; 
          if (dmxviewer_history_counter > 127)dmxviewer_history_counter = 0; 
          u8g2.drawLine(x, y+0, 128-x, y+0);
          u8g2.drawLine(x, y+27, 128-x, y+27);
          for (int ct = 0; ct < 128; ct++){
            u8g2.drawLine(x+ct, y+26-(dmxviewer_history[ct]/10), x+ct+1, y+26-(dmxviewer_history[ct+1]/10) );
            }
          }
      //Channel selector
      x = 0; y = 55;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Src:");   
        if (dmxviewer_source == 0){u8g2.drawStr(x+20,y,"ArtNetA");}
        if (dmxviewer_source == 1){u8g2.drawStr(x+20,y,"ArtNetB");}
      u8g2.drawStr(x+58,y,"Rng:"); 
      display_text_int(x+78,y,dmxviewer_page);
      u8g2.drawStr(x+91,y,"Cha:"); 
      display_text_int(x+111,y,dmxviewer_channel);
      if (menu_edit == 1)  u8g2.drawFrame(x+18,y-7,38,8);
      if (menu_edit == 2)  u8g2.drawFrame(x+76,y-7,13,8);
      if (menu_edit == 3)  u8g2.drawFrame(x+109,y-7,18,8);
      //
      }
  /*IP address 1*/
  if (menu == MENU_NETWORK1){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Node IP Settings 1"); 
      //dhcp
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"DHCP:"); 
      u8g2.setFont(u8g2_font_ncenB10_tr);
      if (dhcp) u8g2.drawStr(x+2,y+12,"Enabled");else u8g2.drawStr(x+2,y+12,"Disabled");
      if (menu_edit == 1)  u8g2.drawFrame(x+0,y+0,70,13);
      }
  /*IP address 2*/
  if (menu == MENU_NETWORK2){
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(0+7, 16, "Node IP Settings 2"); 
      //ip
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"IP: ");   
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,ip[0]);  u8g2.drawStr(x+25,y+12,".");  
      display_text_int(x+29,y+12,ip[1]); u8g2.drawStr(x+52,y+12,".");  
      display_text_int(x+56,y+12,ip[2]); u8g2.drawStr(x+79,y+12,".");  
      display_text_int(x+83,y+12,ip[3]);
      if ((menu_edit <= 4) && (menu_edit > 0))  u8g2.drawFrame(x+0+(((menu_edit-1)%4)*27),y+0,27,13);
      //mask
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Mask: ");  
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,subnet[0]);  u8g2.drawStr(x+25,y+12,".");  
      display_text_int(x+29,y+12,subnet[1]); u8g2.drawStr(x+52,y+12,".");  
      display_text_int(x+56,y+12,subnet[2]); u8g2.drawStr(x+79,y+12,".");  
      display_text_int(x+83,y+12,subnet[3]);
      if (menu_edit >= 5) u8g2.drawFrame(x+0+(((menu_edit-1)%4)*27),y+0,27,13);
      }
  /*Wifi mode*/
  if (menu == MENU_WIFI1){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Wifi Mode"); 
      //wifi radio
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Wifi Radio:"); 
      u8g2.setFont(u8g2_font_ncenB10_tr);
      if (wifi_radio) u8g2.drawStr(x+2,y+12,"Enabled"); else u8g2.drawStr(x+2,y+12,"Disabled");
      if (menu_edit == 1)  u8g2.drawFrame(x+0,y+0,70,13);
      //wifi mode
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Wifi Mode:"); 
      u8g2.setFont(u8g2_font_ncenB10_tr);
      if (wifi_mode_ap) u8g2.drawStr(x+2,y+12,"Access Point"); else u8g2.drawStr(x+2,y+12,"Client");
      if (menu_edit == 2)  u8g2.drawFrame(x+0,y,99,13);
      //
      }
  /*Wifi client*/
  if (menu == MENU_WIFI2){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Wifi Client"); 
      //ssid
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Wifi Client SSID:");  
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(x+2,y+12,wifi_client_ssid); 
      if (menu_edit == 1) u8g2.drawFrame(x+0,y,125,13);
      //client pass
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Wifi Client Password:");  
      u8g2.setFont(u8g2_font_courR10_tf);
      uint8_t shift = 0;
      if (menu_edit > 15){shift = menu_edit - 15;}
      for (int i = 0; i < 15; i++){
          u8g2.setCursor(x+2+8*i,y+12);
          u8g2.print(wifi_client_pass[i+shift]); 
          if (i+shift == menu_edit-2) u8g2.drawFrame(x+2+8*i,y+1,9,12);
          }
      //extender arrows left
      if (shift != 0){
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.setDrawColor(0);
        u8g2.drawBox(x+0, y+0, 8, 17);
        u8g2.setDrawColor(1);
        u8g2.drawStr(x+0,y+12,"< ");  
        }
      //
      }
  /*Art-Net settings 1*/
  if (menu == MENU_ARTNET1){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Art-Net Settings 1"); 
      //net
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Art-Net Net:");  
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,artNetNet); 
      if (menu_edit == 1) u8g2.drawFrame(x+0,y,27,13);
      //subnet
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Art-Net Subnet:"); 
      u8g2.setFont(u8g2_font_ncenB10_tr); 
      display_text_int(x+2,y+12,artNetSub); 
      if (menu_edit == 2) u8g2.drawFrame(x+0,y,19,13);
      //
      }
  /*Art-Net settings 2*/
  if (menu == MENU_ARTNET2){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+7, y, "Art-Net Settings 2"); 
      //universe1
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Art-Net Universe A:");   
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,artNetUniA); 
      if (menu_edit == 1) u8g2.drawFrame(x+0,y,19,13);
      //universe2
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Art-Net Universe B:");   
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,artNetUniB); 
      if (menu_edit == 2) u8g2.drawFrame(x+0,y,19,13);
      //
      }
  /*System settings 1*/
  if (menu == MENU_SYSTEM1){
      x = 0; y = 16;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x+11, y, "System Settings"); 
      //contrast
      x = 4; y = 23;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"LCD contrast:"); 
      u8g2.drawFrame(x+1, y+1, 120, 11);
      u8g2.drawBox(x+1, y+1, (128-2*x)*lcd_contrast/255, 11);
      if (menu_edit == 1)  u8g2.drawFrame(x,y,(128-2*x+2),13);
      //logo screen
      x = 4; y = 43;
      u8g2.setFont(u8g2_font_5x7_tf);
      u8g2.drawStr(x,y,"Logo Screen Time:");  
      u8g2.setFont(u8g2_font_ncenB10_tr);
      display_text_int(x+2,y+12,lcd_splash_time); 
      u8g2.drawStr(x+12,y+12,"Sec.");  
      if (menu_edit == 2) u8g2.drawFrame(x+0,y,47,13);
      //
      }

  //FOOTER
  yield();
  //ip
  x = 0; y = 57;
  u8g2.setFont(u8g2_font_5x7_tf);
  display_text_int(x+0,y+7,ip[0]);
  u8g2.drawStr(x+14,y+7,".");  
  display_text_int(x+18,y+7,ip[1]);
  u8g2.drawStr(x+32,y+7,".");  
  display_text_int(x+36,y+7,ip[2]);
  u8g2.drawStr(x+50,y+7,".");  
  display_text_int(x+54,y+7,ip[3]);
  //subnet number
  uint8_t sub = 0;
  if (subnet[0] == 255) sub += 8;
  if (subnet[1] == 255) sub += 8;
  if (subnet[2] == 255) sub += 8;
  if (subnet[3] == 255) sub += 8;
  u8g2.drawStr(x+69,y+7,"/");  
  display_text_int(x+74,y+7,sub);
  //uptime
  display_text_time_5x7(93, 57, lcd_uptime);
  //
  display_hLine(0, 56, 128);
  display_vLine(91, 56, 8);
  yield();
  u8g2.sendBuffer();  
  }

void display_screen_eeprom_cleared(){
  u8g2.clearBuffer(); 
  //TEXT
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(27, 30, "EEPROM"); 
  u8g2.drawStr(30, 50, "Cleared!"); 
  //SEND TO DISPLAY
  u8g2.sendBuffer();  
  }

void display_screen_wifi_scan(){
  u8g2.clearBuffer(); 
  //TEXT
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(16, 30, "scanning for"); 
  u8g2.drawStr(23, 50, "Networks..."); 
  //SEND TO DISPLAY
  u8g2.sendBuffer();  
  }
  
void display_screen_wifi_client_connect(uint8_t val){
  u8g2.clearBuffer(); 
  //TEXT
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(2, 17, "Connecting..."); 
  u8g2.setFont(u8g2_font_5x7_tf);
  display_text_str(6, 33, wifi_client_ssid ,30);
  //BAR
  uint8_t y = 40;
  u8g2.drawFrame(5,y,118,15);
  u8g2.drawBox(5,y,6*val,15);
  //SEND TO DISPLAY
  u8g2.sendBuffer();  
  }

void display_screen_welcome(){
  uint8_t x = 3;
  uint8_t y = 0;
  //LOGO
  u8g2.clearBuffer();         
  u8g2.setFont(u8g2_font_helvB14_tr);
  u8g2.drawStr(x+0,y+20,"A");  
  u8g2.setFont(u8g2_font_helvR14_tr);
  u8g2.drawStr(x+11,y+20,"U");
  u8g2.drawStr(x+22,y+20,"D");
  u8g2.drawStr(x+33,y+20,"I");
  u8g2.drawStr(x+36,y+20,"O");
  u8g2.setFont(u8g2_font_helvB14_tr);
  u8g2.drawStr(x+49,y+20,"W");  
  u8g2.setFont(u8g2_font_helvR14_tr);
  u8g2.drawStr(x+63,y+20,"E");
  u8g2.drawStr(x+75,y+20,"R");
  
  u8g2.setDrawColor(1);
  u8g2.drawTriangle(x+109, y+5, x+109, y+20, x+119, y+13);
  u8g2.setDrawColor(0);
  u8g2.drawTriangle(x+104, y+5, x+104, y+20, x+114, y+13);
  u8g2.setDrawColor(1);
  u8g2.drawTriangle(x+99, y+5, x+99, y+20, x+109, y+13);
  u8g2.setDrawColor(0);
  u8g2.drawTriangle(x+94, y+5, x+94, y+20, x+104, y+13);

  u8g2.setDrawColor(1);
  u8g2.drawStr(x+86,y+20,"K");
  //NAME
  u8g2.setFont(u8g2_font_ncenB08_tf);
  u8g2.drawStr(x+3,y+30,"ArtGATE Version ");
  u8g2.drawStr(x+99,y+30, SW_VERSION);  
    
  //SEND TO DISPLAY
  u8g2.sendBuffer();          
  }

/*
Helpfunctions
*/
//TIME hh:mm:ss
void display_text_time_16x18(uint8_t x, uint8_t y, int val){  //val == seconds
  u8g2.setFont(u8g2_font_ncenB10_tr);
  uint8_t hours = (val/3600) % 100;
  if (hours < 10){
    display_text_int(x+0, y+7, 0);
    display_text_int(x+8, y+7, hours);
    }
  else display_text_int(x+0, y+7, hours);   //hours
  u8g2.drawStr(x+15,y+6,":");  
  uint8_t mins = (val/60) % 60;
  if (mins < 10){
    display_text_int(x+19, y+7, 0);
    display_text_int(x+27, y+7, mins);
    }
  else display_text_int(x+19, y+7, mins);   //mins
  u8g2.drawStr(x+34,y+6,":");  
  uint8_t secs = val % 60;
  if (secs < 10){
    display_text_int(x+38, y+7, 0);
    display_text_int(x+46, y+7, secs);
    }
  else {
    display_text_int(x+38, y+7, secs);   //secs
    }
  }
  
void display_text_time_5x7(uint8_t x, uint8_t y, int val){  //val == seconds
  u8g2.setFont(u8g2_font_5x7_tf);
  uint8_t hours = (val/3600) % 100;
  if (hours < 10){
    display_text_int(x, y+7, 0);
    display_text_int(x+5, y+7, hours);
    }
  else display_text_int(x, y+7, hours);   //hours
  u8g2.drawStr(x+9,y+7,":");  
  uint8_t mins = (val/60) % 60;
  if (mins < 10){
    display_text_int(x+13, y+7, 0);
    display_text_int(x+18, y+7, mins);
    }
  else display_text_int(x+13, y+7, mins);   //mins
  u8g2.drawStr(x+22,y+7,":");  
  uint8_t secs = val % 60;
  if (secs < 10){
    display_text_int(x+26, y+7, 0); 
    display_text_int(x+31, y+7, secs); 
    }
  else display_text_int(x+26, y+7, secs);   //secs
  }

void display_text_int(uint8_t x, uint8_t y, int val){
  enum {BufSize=6}; 
  char buf[BufSize];
  snprintf (buf, BufSize, "%d", val);
  u8g2.drawStr(x+0,y+0,buf);  
  }

void display_text_str(uint8_t x, uint8_t y, String val, uint8_t len){
    if (len == 0) len = sizeof(val);
    char __text[len];
    val.toCharArray(__text, len);
    u8g2.drawStr(x, y,__text); 
    }
