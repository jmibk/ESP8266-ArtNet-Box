/*
STORAGE MAP
NETWORKING
1         wifi_mode_ap
2         wifi_radio
5         dhcp
20        lcd_contast
21        lcd_splash_time

50        ip[0]
51        ip[1]
52        ip[2]
53        ip[3]
54        broadcast[0]
55        broadcast[1]
56        broadcast[2]
57        broadcast[3]
58        subnet[0]
59        subnet[1]
60        subnet[2]
61        subnet[3]

100-149   wifi_ap_ssid
150-199   wifi_ap_pass
200-249   wifi_client_ssid
250-299   wifi_client_pass

ARTNET
300       nodeName
350       artNetUniA
351       artNetUniB
360       artNetSub
361       artNetNet

WS2812

CHECK
500       Write Check
501       Write Check
502       Check if eeprom is empty
*/


void eeprom_begin(){
  EEPROM.begin(512);
  }

void eeprom_clear(){
  display_screen_eeprom_cleared();
  for (int i = 0 ; i < 512 ; i++) {
    EEPROM.write(i, '\0');
    if (i%50 == 0)delay(100);
    }
    #ifdef DEBUG
      Serial.println("EEPROM content cleared!");
    #endif  
    EEPROM.write(502, '\0');
    EEPROM.commit();
  }


bool eeprom_save() {
  // Wipe our OK and check
  EEPROM.write(500, '\0');
  EEPROM.write(501, '\0');
  EEPROM.commit();
  if(EEPROM.read(500) != '\0'|| EEPROM.read(501) != '\0') {
    #ifdef DEBUG
      Serial.println("fail!");
    #endif
    return false;
    }

  int x;
  //Networking
  EEPROM.write(1, wifi_mode_ap);
  EEPROM.write(2, wifi_radio);
  EEPROM.write(5, dhcp);
  EEPROM.write(20, lcd_contrast);
  EEPROM.write(21, lcd_splash_time);

  EEPROM.write(50, ip[0]);
  EEPROM.write(51, ip[1]);
  EEPROM.write(52, ip[2]);
  EEPROM.write(53, ip[3]);
  EEPROM.write(54, broadcast[0]);
  EEPROM.write(55, broadcast[1]);
  EEPROM.write(56, broadcast[2]);
  EEPROM.write(57, broadcast[3]);
  EEPROM.write(58, subnet[0]);
  EEPROM.write(59, subnet[1]);
  EEPROM.write(60, subnet[2]);
  EEPROM.write(61, subnet[3]);

  for(x = 0; x < 30 && wifi_ap_ssid[x] != '\0'; x++)
    EEPROM.write(100 + x, wifi_ap_ssid[x]);
  EEPROM.write(100 + x, '\0');

  for(x = 0; x < 30 && wifi_ap_pass[x] != '\0'; x++)
    EEPROM.write(150 + x, wifi_ap_pass[x]);
  EEPROM.write(150 + x, '\0');
  
  for(x = 0; x < 30 && wifi_client_ssid[x] != '\0'; x++)
    EEPROM.write(200 + x, wifi_client_ssid[x]);
  EEPROM.write(200 + x, '\0');
  
  for(x = 0; x < 30 && wifi_client_pass[x] != '\0'; x++)
    EEPROM.write(250 + x, wifi_client_pass[x]);
  EEPROM.write(250 + x, '\0');
  //ArtNet
  for(x = 0; x < 30 && nodeName[x] != '\0'; x++)
    EEPROM.write(300+x, nodeName[x]);
  EEPROM.write(300+x, '\0');
  EEPROM.write(350, artNetUniA);
  EEPROM.write(351, artNetUniB);
  EEPROM.write(360, artNetSub);
  EEPROM.write(361, artNetNet);
  EEPROM.write(500, 'O');
  EEPROM.write(501, 'K');
  EEPROM.commit();
  // Verify our OK was written & return false if not
  if(EEPROM.read(500) != 'O' || EEPROM.read(501) != 'K') {
    #ifdef DEBUG
      Serial.println("fail!");
    #endif
    return false;
  }
  
  #ifdef DEBUG
    Serial.println("EEPROM: data stored successfully!");
  #endif

  // Return true if all went well
  return true;
}

bool eeprom_load() {
  int x;
  // If no previous settings, start hotspot
  if(EEPROM.read(502) != 'R') {
    EEPROM.write(502, 'R');
    #ifdef DEBUG
      Serial.println("EEPROM: no memory.");
    #endif
    return false;
  }  
  // Check if we have previous saves.  If not, return false
  if(EEPROM.read(500) != 'O' || EEPROM.read(501) != 'K') {
    #ifdef DEBUG
      Serial.println("EEPROM: memory error detected.");
    #endif 
    return false;
    }
  
  //Networking
  wifi_mode_ap = EEPROM.read(1);
  wifi_radio = EEPROM.read(2);
  dhcp = EEPROM.read(5);
  lcd_contrast = EEPROM.read(20);
  lcd_splash_time = EEPROM.read(21);
  
  ip[0] = EEPROM.read(50);
  ip[1] = EEPROM.read(51);
  ip[2] = EEPROM.read(52);
  ip[3] = EEPROM.read(53);
  broadcast[0] = EEPROM.read(54);
  broadcast[1] = EEPROM.read(55);
  broadcast[2] = EEPROM.read(56);
  broadcast[3] = EEPROM.read(57);
  subnet[0] = EEPROM.read(58);
  subnet[1] = EEPROM.read(59);
  subnet[2] = EEPROM.read(60);
  subnet[3] = EEPROM.read(61);

  //AP SSID
  for(x = 0; x < 30; x++) {
    wifi_ap_ssid[x] = EEPROM.read(100 + x);
    if (wifi_ap_ssid[x] == '\0')
      break;
  }

  //AP PASS
  for(x = 0; x < 30; x++) {
    wifi_ap_pass[x] = EEPROM.read(150 + x);
    if (wifi_ap_pass[x] == '\0')
      break;
  }
  wifi_ap_pass[x] = '\0';

  //CLIENT SSID
  for(x = 0; x < 30; x++) {
    wifi_client_ssid[x] = EEPROM.read(200 + x);
    if (wifi_client_ssid[x] == '\0')
      break;
  }
  wifi_client_ssid[x] = '\0';

  //CLIENT PASS
  for(x = 0; x < 30; x++) {
    wifi_client_pass[x] = EEPROM.read(250 + x);
    if (wifi_client_pass[x] == '\0')
      break;
  }
  wifi_client_pass[x] = '\0';

  
  
  //ArtNet
  for(x = 0; x < 30; x++) {
    nodeName[x] = EEPROM.read(300+x);
    if (nodeName[x] == '\0')
      break;
    }
  nodeName[x] = '\0';

  artNetUniA = EEPROM.read(350);
  artNetUniB = EEPROM.read(351);
  artNetSub = EEPROM.read(360);
  artNetNet = EEPROM.read(361);
  
  #ifdef DEBUG
    Serial.println("EEPROM: data loaded successfully");
  #endif

  artNetUniA = artNetUniA;
  artNetUniB = artNetUniB;

  // Return
  return true;
}

