void encoder_start(){
  //encoder = new ESPEncoder(14, 12, 13, 4);  //EncA, EncB, Switch, Rastergeorge speicher
  encoder = new ESPEncoder(16, 0, 3, 4);  //EncA, EncB, Switch, Rastergeorge speicher
  encoder->setAccelerationEnabled(true);
  last = -1;
  }

void encoder_service(){
  /*Encoder Rotate*/
  if (menu_edit == 0){menu += encoder->getValue(); if (menu > MENU_ITEMS)menu = MENU_STATUS;if (menu <= 0)menu = MENU_ITEMS;}        //scroll thru menu
  //DMX VIEWER
  if ((menu == MENU_DMXVIEW) && (menu_edit == 1)){dmxviewer_source += encoder->getValue();if(dmxviewer_source > 1)dmxviewer_source = 1;}
  if ((menu == MENU_DMXVIEW) && (menu_edit == 2)){dmxviewer_page += encoder->getValue();if((dmxviewer_page > 25)&&(dmxviewer_page<200))dmxviewer_page = 0;if(dmxviewer_page>=200)dmxviewer_page=25;}
  if ((menu == MENU_DMXVIEW) && (menu_edit == 3)){
    dmxviewer_channel += encoder->getValue();if((dmxviewer_channel > 512)&&(dmxviewer_channel<1000))dmxviewer_channel = 1;if(dmxviewer_channel>=1000)dmxviewer_channel=512;
    if (encoder->getValue() != 0){
      for (int i = 0; i < 255; i++)dmxviewer_history[i] = 0;
      dmxviewer_history_counter = 0;
      }
    }
  //IP1
  if ((menu == MENU_NETWORK1) && (menu_edit == 1)) {if (encoder->getValue() != 0){dhcp = !dhcp;} }
  //IP 2
  if ((menu == MENU_NETWORK2) && (menu_edit == 1)) {ip[0] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 2)) {ip[1] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 3)) {ip[2] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 4)) {ip[3] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 5)) {subnet[0] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 6)) {subnet[1] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 7)) {subnet[2] += encoder->getValue();}
  if ((menu == MENU_NETWORK2) && (menu_edit == 8)) {subnet[3] += encoder->getValue();}
  //WIFI MODE
  if ((menu == MENU_WIFI1) && (menu_edit == 1)) {if (encoder->getValue() != 0){wifi_radio = !wifi_radio;} }
  if ((menu == MENU_WIFI1) && (menu_edit == 2)) {if (encoder->getValue() != 0){wifi_mode_ap = !wifi_mode_ap;} }
  //WIFI CLIENT APS
  if ((menu == MENU_WIFI2) && (menu_edit == 1)) {
      wifi_ap_num += encoder->getValue(); 
      if ((wifi_ap_num > wifi_aps_found) && (wifi_ap_num < 100)){wifi_ap_num = 0;}
      if (wifi_ap_num >= 100){wifi_ap_num = wifi_aps_found;}
      char ClientssidChar[30];
      WiFi.SSID(wifi_ap_num).toCharArray(ClientssidChar, 30);
      strncpy(wifi_client_ssid, ClientssidChar, 30);
      }
  if ((menu == MENU_WIFI2)&&(menu_edit >= 2)){
      wifi_client_pass[menu_edit-2] += encoder->getValue();
      if ((wifi_client_pass[menu_edit-2] < 32)&&(wifi_client_pass[menu_edit-2] > 0)){wifi_client_pass[menu_edit-2] = 122;}
      if (wifi_client_pass[menu_edit-2] > 122){wifi_client_pass[menu_edit-2] = 32;}
      //Serial.println(wifi_client_pass[menu_edit-2], DEC);
      }
  //ARTNET NET SUBNET
  if ((menu == MENU_ARTNET1) && (menu_edit == 1)) {artNetNet += encoder->getValue();if((artNetNet>127)&&(artNetNet<200))artNetNet = 0;if(artNetNet>=200)artNetNet = 127;}
  if ((menu == MENU_ARTNET1) && (menu_edit == 2)) {artNetSub += encoder->getValue();if((artNetSub>15)&&(artNetSub<200))artNetSub = 0;if(artNetSub >=200)artNetSub = 15;}
  //ARTNET UNIVERSES
  if ((menu == MENU_ARTNET2) && (menu_edit == 1)) {artNetUniA += encoder->getValue();if((artNetUniA>15)&&(artNetUniA<200))artNetUniA = 0;if(artNetUniA>=200)artNetUniA = 15;}
  if ((menu == MENU_ARTNET2) && (menu_edit == 2)) {artNetUniB += encoder->getValue();if((artNetUniB>15)&&(artNetUniB<200))artNetUniB = 0;if(artNetUniB>=200)artNetUniB = 15;}
  //SYSTEM
  if ((menu == MENU_SYSTEM1) && (menu_edit == 1)) {lcd_contrast += encoder->getValue()*10; u8g2.setContrast(lcd_contrast);}
  if ((menu == MENU_SYSTEM1) && (menu_edit == 2)) {lcd_splash_time += encoder->getValue();if ((lcd_splash_time > 5)&&(lcd_splash_time<200))lcd_splash_time = 0; if(lcd_splash_time>=200)lcd_splash_time=5;}

  /*Button Press*/
  ESPEncoder::Button b = encoder->getButton();
  if (b != ESPEncoder::Open) {
    switch (b) {
      case ESPEncoder::Released:
        if (menu == MENU_STATUS)eeprom_clear();
        break;
        
      case ESPEncoder::Held:
        if ( (menu == MENU_WIFI2) && (menu_edit >= 2)){wifi_client_pass[menu_edit-2] = 0;}
        break;
        
      case ESPEncoder::Clicked:
        //NETWORK SETTINGS
        if ((menu == MENU_NETWORK1)||(menu == MENU_NETWORK2)){
            if (dhcp){}
            else {
              IPAddress gw(ip[0], 0, 0, 1);
              WiFi.config(ip, gw, subnet);
              #ifdef DEBUG
                  Serial.println("Set new network address");
              #endif
              }
            }
         //WIFI 
          if ((menu == MENU_WIFI2)&&(menu_edit > 2)){   //if client password is empty char
            if (wifi_client_pass[menu_edit-2] == 0){
                for (int i = menu_edit-2; i < 30; i++){
                    wifi_client_pass[i] = 0;
                    }
                wifi_start();
                ethernet_start();
                menu_edit = -1;
                eeprom_save();
                }
            } 
        //SAVE AND BACK TO MAIN
        menu_edit++; 
        if (menu_edit > menu_edit_items[menu]){
            menu_edit = 0;
            if (menu > 2) eeprom_save();
            }
        //RESTART NETWORKS
        if ( ((menu == MENU_WIFI1) || (menu == MENU_WIFI2)) && (menu_edit == 0)) {wifi_start();}    
        
      ////////////////////////    
        //not only when saving...  
       if ((menu == MENU_WIFI2)&&(menu_edit == 1)){   //scan wifi
          display_screen_wifi_scan();
          Wifi_Get_Networks();
          }
        break;
      case ESPEncoder::Pressed:
        break;
      case ESPEncoder::DoubleClicked:
          if (menu_edit == 0){
            menu = MENU_STATUS;
            dmxviewer_channel = 0;
            return;
            }
          menu_edit = 0;
          eeprom_load();
          //encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
          //Serial.print("  Acceleration is ");
          //Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
      }
    }  
  // 
  }
