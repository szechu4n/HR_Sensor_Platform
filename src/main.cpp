#include "main.h"

const uint8_t debug = 0;

int main(){
  PlatformInit();
  ErrorSet(PowerOnSelfTest());
  while(1){
    SleepModeIdle();
  }
  return 0;
}

void serialEvent(){
  int i = 0;
  uint8_t serialRxBuffer[1000];
  while(Serial.available()){
    serialRxBuffer[i++] = Serial.read();
  }
  ErrorSet(CRCCheck(serialRxBuffer, i)); // i = len
  CmdInvoker(serialRxBuffer, i);
}


void ADCInit(){
  
}

void SerialInit(){

}

void PlatformInit(){
  /********************************************
   * Initializes platform, necessary 
   * peripherals, and prepares for POST. 
   *******************************************/ 
  CRCInit();
  ADCInit();
  SerialInit();
}

ErrorCode PowerOnSelfTest(){
  return NO_ERROR;
}

void SleepModeIdle(){
  set_sleep_mode(SLEEP_MODE_IDLE)
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
}

void ProcessSystem(){
  uint16_t ecg_diff = 0, pcg1_diff = 0, pcg2_diff = 0;
  ReadADC(ecg_diff, pcg1_diff, pcg2_diff);
  if(debug)
    WriteToSDCard();
  
  uint8_t* ecg_diff_ptr  = (uint8_t*)&ecg_diff; // really worried about this causing seg fault lol
  uint8_t* pcg1_diff_ptr = (uint8_t*)&pcg1_diff;
  uint8_t* pcg2_diff_ptr = (uint8_t*)&pcg2_diff;
  // add rtc stuff here

  uint8_t msg [7]; 
  msg[0] = 0x55;
  msg[1] = ecg_diff_ptr[1];
  msg[2] = ecg_diff_ptr[0];
  msg[3] = pcg1_diff_ptr[1];
  msg[4] = pcg1_diff_ptr[0];
  msg[5] = pcg2_diff_ptr[1];
  msg[6] = pcg2_diff_ptr[0];

  crc CrcData = CRCFast(msg, 7);

  Serial.print(0x55);
  Serial.print(ecg_diff);
  Serial.print(pcg1_diff);
  Serial.print(pcg2_diff);
  Serial.println(CrcData);
  
}

void ReadADC(uint16_t ecg_diff, uint16_t pcg1_diff, uint16_t pcg2_diff){

}