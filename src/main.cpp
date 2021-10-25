#include "main.h"

void platform_init();
ErrorCode PowerOnSelfTest();
//void StartSynchronizationProcess();
void SleepModeIdle();

int main(){
  platform_init();
  ErrorSet(globalErrorCode,PowerOnSelfTest());
  //StartSynchronizationProcess(); // TODO: add library for this
  while(1){
    SleepModeIdle();
  }
  return 0;
}

void platform_init(){
  /********************************************
   * Initializes platform, necessary 
   * peripherals, and prepares for POST. 
   *******************************************/ 
  CRCInit();

}

void SleepModeIdle(){
  set_sleep_mode(SLEEP_MODE_IDLE)
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
}

void serialEvent(){
  int i = 0;
  while(Serial.available()){
    serialRxBuffer[i++] = Serial.read();
  }
  ErrorSet(globalErrorCode,CRCCheck(serialRxBuffer, i)); // i = len
  CmdInvoker(serialRxBuffer, i);
}