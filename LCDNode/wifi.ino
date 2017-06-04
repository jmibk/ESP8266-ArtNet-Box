void wifi_start(){
  WiFi.persistent(false);
  if (wifi_radio){
    if (wifi_mode_ap){  //AP Mode
        Wifi_AP();
        }
    else{
        if (Wifi_Client() == false){
          wifi_mode_ap = true;
          wifi_start();
          }
        }
    }
  else {
    WiFi.mode(WIFI_OFF);
    }
  }

void Wifi_Get_Networks(){
  #ifdef DEBUG
    Serial.println("Scanning for WIFI networks...");
  #endif
  wifi_aps_found = WiFi.scanNetworks();
    if (wifi_aps_found == -1) {
      #ifdef DEBUG
        Serial.println("No wifi networks found!");
      #endif
      }
    //print the list of networks seen:
    int i;
    for (i = 0; i < wifi_aps_found; i++){
      if (i >= MAX_WIFI_APS)break;
      wifi_aps[i] = WiFi.SSID(i);
      }
  }

bool Wifi_Client() {

  #ifdef DEBUG
    Serial.print("Wifi Client: Login SSID: "); Serial.println(wifi_client_ssid);
    Serial.print("Wifi Client: Login Password: "); Serial.println(wifi_client_pass);
  #endif
  
  // Connect wifi
  WiFi.begin(wifi_client_ssid, wifi_client_pass);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("ArtNet Node");
  // Wait for WiFi to connect
  uint8_t x = 20;
  while ( (WiFi.status() != WL_CONNECTED) && (x-- != 0) ){
    delay(500);
    display_screen_wifi_client_connect(20-x);
    #ifdef DEBUG
      Serial.print(".");
    #endif
    }
      #ifdef DEBUG
        Serial.println("");
      #endif
      if (WiFi.status() == WL_CONNECTED){
        #ifdef DEBUG
          Serial.println("Wifi Client: Connected Successfully");
        #endif
        }
      else {
        #ifdef DEBUG
          Serial.println("Wifi Client: Failed to connect");
        #endif
        return false;
        }
    

  setMacAddress();
  
  //dhcp 
  if (dhcp != 1) {
    IPAddress gw(ip[0], 0, 0, 1);
    WiFi.config(ip, gw, subnet);
  } else {
  
    ip = WiFi.localIP();  
    subnet = WiFi.subnetMask();
    #ifdef DEBUG
      Serial.print("Wifi Client: IP: "); Serial.println(ip);
      Serial.print("Wifi Client: Subnet: "); Serial.println(subnet);
    #endif
  }
  //broadcast address
  setBroadcastAddr();
  return true;
  }

String Wifi_get_ap_name(){
  // generate the AP ssid & password
  String ssid = wifi_ap_ssid;
  ssid += '_';
  ssid += String(ESP.getChipId());
  return ssid;
  }

void Wifi_AP() {
  // generate the AP ssid & password
  String ssid = Wifi_get_ap_name();

  // Turn strings into char array for the functions below
  char ssidChar[ssid.length()];
  ssid.toCharArray(ssidChar, ssid.length());
  
  #ifdef DEBUG
    Serial.print("Wifi AP: SSID: "); Serial.println(ssidChar);
    Serial.print("Wifi AP: Password: "); Serial.println(wifi_ap_pass);
  #endif
  
  // start softAP
  WiFi.mode(WIFI_AP);   //WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF
  WiFi.softAP(ssidChar, wifi_ap_pass);

  //set ip addresses
  WiFi.softAPConfig(ip, ip, subnet);
  setBroadcastAddr();
  setMacAddress();
  #ifdef DEBUG
    Serial.print("Wifi AP: IP: "); Serial.println(ip);
    Serial.print("Wifi AP: Mask: "); Serial.println(subnet);
    Serial.print("Wifi AP: Broadcast: "); Serial.println(broadcast);
    Serial.print("Wifi AP: MAC address: "); Serial.println(MAC_address);
  #endif
  }


void setMacAddress() {
  char MAC_char[30] = "";
  
  WiFi.macAddress(MAC_array);
  
  // Format the MAC address into string
  sprintf(MAC_char, "%02X", MAC_array[0]);
  for (int i = 1; i < 6; ++i)
    sprintf(MAC_char, "%s : %02X", MAC_char, MAC_array[i]);
  MAC_address = String(MAC_char);
}

void setBroadcastAddr() {
  broadcast = {~subnet[0] | (ip[0] & subnet[0]), ~subnet[1] | (ip[1] & subnet[1]), ~subnet[2] | (ip[2] & subnet[2]), ~subnet[3] | (ip[3] & subnet[3])};
}
