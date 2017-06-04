//COMMON
//#define DEBUG
#define IRQ_ARTNET_UPDATE 50 //1ms (20x/s = 50)
#define IRQ_LCD_CONTENT_UPDATE  1000  //1ms (1/s = 1000)
#define SW_VERSION "1.0"
int timer = 0;   //for call in interrupt

//EEPROM
#include <EEPROM.h>

//LCD
#include <U8g2lib.h>
#include <SPI.h>
////U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  //Arduino
//U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=D0*/ 16, /* data=D1*/ 5, /* cs=*/ 20, /* dc=D2*/ 4, /* reset=D3*/ 0);      //ESP8266
////U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ 20, /* dc=*/ 12 );       //ESP8266 HW SPI
/*clock=SCK, data=SDA, cs=CS (GND), dc=RS, reset=RST*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
////U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

uint8_t   lcd_contrast = 170;
uint8_t   lcd_splash_time = 3;    //seconds
//variables for lcd displaying
int       lcd_uptime = 0;
int       lcd_wifi_rssi = 0;
String    lcd_wifi_mode = "";
char      lcd_wifi_ssid[30] = "";
int       lcd_artnet_time_a = 0;
int       lcd_artnet_time_b = 0;
int       lcd_artnet_time_last_artdmx = 0;

//DMXVIEWER
uint8_t dmxviewer_source = 0;
uint8_t dmxviewer_page = 0;
uint16_t dmxviewer_channel = 0;
uint8_t dmxviewer_history_counter;
uint8_t dmxviewer_history[256];

//ENCODER
#include <ESPEncoder.h>
#define ENC_DECODER (1 << 2)
#define ENC_HALFSTEP 4
ESPEncoder *encoder;
int16_t last, value;
#define MENU_ITEMS  10
int menu_edit = 0;
int menu = 1;
int menu_edit_items[MENU_ITEMS+1] = {0, 0, 0, 3, 1, 8, 2, 31, 2, 2, 2};     //first is zero
#define MENU_STATUS   1
#define MENU_LAST_ARDDMX  2
#define MENU_DMXVIEW  3
#define MENU_NETWORK1 4
#define MENU_NETWORK2 5
#define MENU_WIFI1    6
#define MENU_WIFI2    7
#define MENU_ARTNET1  8
#define MENU_ARTNET2  9
#define MENU_SYSTEM1   10
//main, artDMX, DMXViewer, DHCP, IP, WifiMode, WIfiClient, ArtNet1, ArtNet2, System

//NETWORK
#include <ESP8266WiFi.h>
#include <Ethernet.h>
bool wifi_mode_ap = false;  //true = AP, false = client 
bool wifi_radio = true;      //Wifi ON or OFF
bool dhcp = 1;
//byte MAC_array[6];
byte MAC_array[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
String MAC_address;
IPAddress ip(10, 0, 0, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress broadcast(ip[0], 255, 255, 255);
char wifi_client_ssid[30] = "Martin Router King";
char wifi_client_pass[30] = "affe123456";
char wifi_ap_ssid[30] = "ArtGATE";
char wifi_ap_pass[30] = "affe123456";
uint8_t wifi_connect_timer = 0;
#define MAX_WIFI_APS 30
String wifi_aps[MAX_WIFI_APS];
uint8_t wifi_ap_num = 0;
uint8_t wifi_aps_found = 0;

//ARTNET
#include <WiFiUdp.h>
#include <EthernetUdp.h>
#define ARTNET_PORT 6454
#define ARTNET_BUFFER_MAX 600
#define ARTNET_REPLY_SIZE 239
#define ARTNET_ADDRESS_OFFSET 18
#define ARTNET_ARTDMX 0x5000
#define ARTNET_ARTPOLL 0x2000
unsigned char packetBuffer[ARTNET_BUFFER_MAX];
char nodeName[30] = "Art-Net Node 1";
uint8_t artNetUniA = 0;
uint8_t artNetUniB = 1;
uint8_t artNetSub = 0;
uint8_t artNetNet = 0;
WiFiUDP eUDP;
EthernetUDP UDP;
int artNetUniA_LastReceived = 0;
int artNetUniB_LastReceived = 0;
int artNet_LastReceived_Net = 0;
int artNet_LastReceived_SubNet = 0;
int artNet_LastReceived_Uni = 0;
int artNet_LastReceived_Time = 0;

//DMX
#include <espDMX.h>
enum dmxStates {DMX_STOPPED, DMX_STARTED};
bool dmxStateA = DMX_STOPPED;
bool dmxStateB = DMX_STOPPED;
bool newDataA = 0;
bool newDataB = 0;
byte* dmxDataA;       //pointer
byte* dmxDataB;       //pointer
  //dmxN.end();
  //dmxN.pause();
  //dmxN.unPause();
  //dmxN.clearChans();
  //dmxN.getChans();
  //dmxN.begin();
  //dmxN.begin(ledPin);
  //dmxN.begin(ledPin, ledIntensity);
  //dmxN.ledIntensity(newIntensity);
  //dmxN.setChans(data, numChans, startChan);
  //dmxN.setChans(data, numChans);
  //dmxN.setChans(data);
