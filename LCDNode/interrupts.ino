void ISR_1ms() {
  timer++;
  /*ENCODER*/
  encoder->service();
  /*RECEIVE ARTNET*/
  if (timer % IRQ_ARTNET_UPDATE == 0){
      ArtNetCheckReceiveWifi();
      ArtNetCheckReceiveEthernet();
      }
  /**/
  if (timer % IRQ_LCD_CONTENT_UPDATE == 0){display_data_update();}
  /*TIMER RESET*/
  timer0_write(ESP.getCycleCount() + 80000L); // 80MHz == 1msec
  }

void interrupts_start(){
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(ISR_1ms);
  timer0_write(ESP.getCycleCount() + 80000L); // 80MHz == 1msec
  interrupts();
  }
