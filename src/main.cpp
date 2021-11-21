#include "main.h"

const uint8_t debug = 0;

ADC* adc = new ADC;
const int readPin_A1 = A1;
const int readPin_A2 = A2;
const int readPin_A9 = A9;

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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(readPin_A1, INPUT);
  pinMode(readPin_A2, INPUT);

  adc->adc0->setAveraging(16);
  adc->adc0->setResolution(16);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);

  adc->adc1->setAveraging(16);
  adc->adc1->setResolution(16);
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
}

void SerialInit(){
  Serial.begin(115200);
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
  static int16_t ecg_diff;
  static uint16_t pcg1_diff, pcg2_diff;
  static time_t sample_time;
  ReadADC(ecg_diff, pcg1_diff, pcg2_diff, sample_time);
  if(debug)
    WriteToSDCard();
  
  uint8_t* ecg_diff_ptr  = (uint8_t*)&ecg_diff; // really worried about this causing seg fault lol
  uint8_t* pcg1_diff_ptr = (uint8_t*)&pcg1_diff;
  uint8_t* pcg2_diff_ptr = (uint8_t*)&pcg2_diff;
  // add rtc stuff here

  uint8_t msg [8]; 
  msg[0] = 0x55;
  msg[1] = ecg_diff_ptr[1];
  msg[2] = ecg_diff_ptr[0];
  msg[3] = pcg1_diff_ptr[1];
  msg[4] = pcg1_diff_ptr[0];
  msg[5] = pcg2_diff_ptr[1];
  msg[6] = pcg2_diff_ptr[0];
  msg[7] = CRCFast(msg, 7);

  WriteSerial(msg,8);  
}

void ReadADC(int16_t &ecg_diff, uint16_t &pcg1_diff, uint16_t &pcg2_diff, uint16_t &sample_time){
  static uint16_t prev_value_ecg = 0;
  noInterrupts();
  sample_time = Teensy3Clock.get();
  uint16_t value_A9 = adc->adc1->analogRead(readPin_A9);
  uint16_t value_A1 = adc->adc0->analogRead(readPin_A1);
  uint16_t value_A2 = adc->adc1->analogRead(readPin_A2);
  interrupts();
  ecg_diff = value_A9 - prev_value_ecg;
  prev_value_ecg = value_A9;
  pcg1_diff = value_A1;
  pcg2_diff = value_A2;
}

void WriteSerial(uint8_t msg[], uint8_t len)
{
  for(int i = 0; i < len-1; i++)
  {
    Serial.print(msg[i]);
  }
  Serial.println(msg[len-1]);
}