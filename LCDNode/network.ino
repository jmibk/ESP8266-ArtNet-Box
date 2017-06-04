void ethernet_start(){
  if (!wifi_radio){
      MAC_array[2] = ip[0];
      MAC_array[3] = ip[1];
      MAC_array[4] = ip[2];
      MAC_array[5] = ip[3];
      if (dhcp){
          Ethernet.begin(MAC_array); 
          ip = Ethernet.localIP();  
          #ifdef DEBUG
          Serial.print("Ethernet IP: ");
          Serial.println(Ethernet.localIP());
          #endif
          }
      
      else Ethernet.begin(MAC_array, ip);
      }
   else Ethernet.begin(MAC_array, ip);
  }
  

