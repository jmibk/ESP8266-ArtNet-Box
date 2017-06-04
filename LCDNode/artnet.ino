void artnet_start(){
  eUDP.begin(ARTNET_PORT);
  UDP.begin(ARTNET_PORT);
  ArtPollReply();
  }

void ArtNetCheckReceiveEthernet(){
  int packetSize = UDP.parsePacket();
  // If any packets are received
  if (packetSize) {
      UDP.read(packetBuffer, ARTNET_BUFFER_MAX);
      parseUDPPacket(packetBuffer);
      }
  }
  
void ArtNetCheckReceiveWifi(){
  int packetSize = eUDP.parsePacket();
  // If any packets are received
  if (packetSize) {
      eUDP.read(packetBuffer,ARTNET_BUFFER_MAX);
      parseUDPPacket(packetBuffer);
      }
  }

void parseUDPPacket(unsigned char* pbuff){
    //OPCODE
    int opcode = 0;
    String test = String((char*)packetBuffer);
    if ( test.equals("Art-Net") ) {
        if ( packetBuffer[11] >= 14 ) {            //protocol version [10] hi byte [11] lo byte
            opcode = packetBuffer[9] *256 + packetBuffer[8];  //opcode lo byte first
            }
        }
    //
    if (opcode == ARTNET_ARTDMX)  {artDMXReceived(packetBuffer);}
    if (opcode == ARTNET_ARTPOLL) {ArtPollReply();}
  }

/* artDMXReceived()
 *  Handles ArtNet DMX packets
 */
void artDMXReceived(unsigned char* pbuff) {
  artNet_LastReceived_Net = pbuff[15];
  artNet_LastReceived_SubNet = pbuff[14] >> 4;
  artNet_LastReceived_Uni = pbuff[14] & 0xF;
  artNet_LastReceived_Time = millis();

  // Check Subnet
  if ( (pbuff[14] >> 4) != artNetSub )
    return;

  // Which universe is this packet for (Z = neither)
  char uni = ((pbuff[14] & 0xF) == artNetUniA) ? 'A' : (((pbuff[14] & 0xF) == artNetUniB) ? 'B' : 'Z');
  
  // If not correct Artnet Universe
  if ( uni == 'Z' )
    return;
    
  #ifdef DEBUG
    //Serial.print("Artnet Uni ");
    //Serial.print(uni);
    //Serial.println(" Received");
  #endif
  
  // Number of dimmers hi byte first
  int numberOfDimmers = pbuff[16]*256 + pbuff[17];

  // If there's new data, output DMX
  if ( numberOfDimmers > 0 ) {
     if ( uni == 'A'){
        #ifndef DEBUG
          dmxA.setChans(&pbuff[ARTNET_ADDRESS_OFFSET], numberOfDimmers);
        #endif
        artNetUniA_LastReceived = millis();
        }
    if ( uni == 'B'){
        dmxB.setChans(&pbuff[ARTNET_ADDRESS_OFFSET], numberOfDimmers);
        artNetUniB_LastReceived = millis();
        }
  }
}

void ArtPollReply() {
  unsigned char  replyBuffer[ARTNET_REPLY_SIZE];
  int i;

  #ifdef DEBUG
    //Serial.println("Sending Artnet Reply");
  #endif
  
  for ( i=0; i<ARTNET_REPLY_SIZE; i++ ) {
    replyBuffer[i] = 0;
  }
  replyBuffer[0] = 'A';
  replyBuffer[1] = 'r';
  replyBuffer[2] = 't';
  replyBuffer[3] = '-';
  replyBuffer[4] = 'N';
  replyBuffer[5] = 'e';
  replyBuffer[6] = 't';
  replyBuffer[7] = 0;
  replyBuffer[8] = 0;        //op code lo-hi
  replyBuffer[9] = 0x21;      
  replyBuffer[10] = ip[0];      //ip address
  replyBuffer[11] = ip[1];
  replyBuffer[12] = ip[2];
  replyBuffer[13] = ip[3];
  replyBuffer[14] = 0x36;    // port lo first always 0x1936
  replyBuffer[15] = 0x19;
  replyBuffer[16] = 0;      //firmware hi-lo
  replyBuffer[17] = 0;
  replyBuffer[18] = highByte(subnet+1);  //subnet hi-lo
  replyBuffer[19] = lowByte(subnet+1);
  replyBuffer[20] = 0;      //oem hi-lo
  replyBuffer[21] = 0;
  replyBuffer[22] = 0;      // ubea
  replyBuffer[23] = 160;    // status 1
  replyBuffer[24] = 't';    //     ESTA Code (2 bytes)
  replyBuffer[25] = 'm';

                            //short name
  for (int x = 26; x < 43; x++) {
    replyBuffer[x] = nodeName[x-26];
    if (nodeName[x-26] == 0)
      break;
  }
  
                            //long name
  for (int x = 44; x < 70; x++) {
    replyBuffer[x] = nodeName[x-44];
    if (nodeName[x-44] == 0)
      break;
  }
  
  replyBuffer[173] = 2;    //number of ports
  replyBuffer[174] = 128;  //Port 1 can output DMX from network
  replyBuffer[175] = 128;  //Port 2 can output DMX from network
  
  replyBuffer[182] = 128; //Port 1 good output (128 = data being transmitted)
  replyBuffer[183] = 128; //Port 2 good output (128 = data being transmitted)
  
  replyBuffer[190] = artNetUniA + 16*artNetSub;  // Port 1 address
  replyBuffer[191] = artNetUniB + 16*artNetSub;  // Port 2 address

  // MAC Address
  for (int x = 0; x < 6; x++)
    replyBuffer[201 + x] = MAC_array[x];

  replyBuffer[212] = (dhcp) ? 15 : 13;  // status 2

  // Send packet
  eUDP.beginPacket(broadcast, ARTNET_PORT);
  int test = eUDP.write(replyBuffer,ARTNET_REPLY_SIZE);
  eUDP.endPacket();
  UDP.beginPacket(broadcast, ARTNET_PORT);
  int test2 = UDP.write(replyBuffer,ARTNET_REPLY_SIZE);
  UDP.endPacket();
}
