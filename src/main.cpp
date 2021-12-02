#include "main.h"

uint8_t serialMsg[6];
uint8_t pwrToggle;
byte ledToggle = 0;

ADC* adc = new ADC;
Logger* logger;

int main(){
  pinMode(buttonStartPin_D12, INPUT);
  int buttonState = 0;
  while(buttonState == LOW){
    buttonState = digitalRead(buttonStartPin_D12);
  }
  PlatformInit();
  #if DEBUG == 1
  logger->logEvent("System Fully Initialized");
  // #elif DEBUG == 2
  // Serial.println("System Fully Initialized");
  // logger->logEvent("System Fully Initialized");
  #endif
  ErrorSet(PowerOnSelfTest());
  while(1){
    if(Serial.available()){
      while(Serial.available()){
        int i = 0;
        for(i = 0; i < 6; i++)
        {
          serialMsg[i++] = Serial.read();
        }
        logger->logEvent(serialMsg,i);
        CmdInvoker(serialMsg);
      }
    }
    else{
      digitalWrite(ledSerialPin_D7, HIGH);
    }
    //SleepModeIdle();
  }
  return 0;
}

void ADCInit(){
  pinMode(readPin_A1, INPUT);
  pinMode(readPin_A9, INPUT);

  adc->adc0->setAveraging(0);
  adc->adc0->setResolution(16);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  //adc->adc0->enableInterrupts(adc0_isr);

  adc->adc1->setAveraging(0);
  adc->adc1->setResolution(16);
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS);
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  //adc->adc1->enableInterrupts(adc1_isr);
}


void SerialInit(){
  Serial.begin(115200);
  pinMode(ledSerialPin_D7, OUTPUT);
  digitalWrite(ledSerialPin_D7, HIGH);
}

void ButtonInit(){
  attachInterrupt(buttonStartPin_D12, ButtonInterrupt, RISING);
  pwrToggle = 0;
}

void PlatformInit(){
  /********************************************
   * Initializes platform, necessary 
   * peripherals, and prepares for POST. 
   *******************************************/ 
  pinMode(ledErrorPin_D8, OUTPUT);
  char filename[] = "log.txt";
  logger = new Logger(filename);
  SerialInit();
  #if DEBUG == 1
  logger->logEvent("Serial Initialized");
  #elif DEBUG == 2
  Serial.println("Serial Initialized");
  logger->logEvent("Serial Initialized");
  #endif
  ButtonInit();
  #if DEBUG == 1
  logger->logEvent("Button Initialized");
  #elif DEBUG == 2
  Serial.println("Button Initialized");
  logger->logEvent("Button Initialized");
  #endif
  CRCInit();
  #if DEBUG == 1
  logger->logEvent("CRC Initialized");
  #elif DEBUG == 2
  Serial.println("CRC Initialized");
  logger->logEvent("CRC Initialized");
  #endif
  ADCInit();
  #if DEBUG == 1
  logger->logEvent("ADC Initialized");
  #elif DEBUG == 2
  Serial.println("ADC Initialized");
  logger->logEvent("ADC Initialized");
  #endif
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
  int_fast16_t ecg_diff;
  uint_fast16_t pcg_diff;
  time_t sample_time;
  ledToggle ^=1;
  digitalWriteFast(ledErrorPin_D8,ledToggle);
  ReadADC(ecg_diff, pcg_diff, sample_time);
  
  #if DEBUG == 1
  logger->logEvent("Data Sampled");
  #endif
  
  uint_fast8_t ecg[2];
  uint_fast8_t pcg[2];
  uint_fast8_t time[4];

  ecg[0] = ecg_diff & 255;
  ecg[1] = (ecg_diff >> 8) & 255;

  pcg[0] = pcg_diff & 255;
  pcg[1] = (pcg_diff >> 8) & 255;

  time[0] = sample_time & 255;
  time[1] = (sample_time >> 8) & 255;
  time[2] = (sample_time >> 16) & 255;
  time[3] = (sample_time >> 24) & 255;
  
  // add rtc stuff here

  uint8_t msg [10]; 
  msg[0] = 0x55;
  msg[1] = time[3];
  msg[2] = time[2];
  msg[3] = time[1];
  msg[4] = time[0];
  msg[5] = ecg[1];
  msg[6] = ecg[0];
  msg[7] = pcg[1];
  msg[8] = pcg[0];
  msg[9] = CRCFast(msg, 9);

  //logger->logEvent("Data Message Available");
  //logger->logEvent(msg,10);

  Serial.write(msg,10);
}

void ProcessSystemFast(){
  int_fast16_t ecg_diff;
  uint_fast16_t pcg_diff;
  
  static int_fast16_t prev_value_ecg = 0;
  static int_fast16_t prev_value_pcg = 0;
  int_fast16_t value_A9 = adc->adc1->analogRead(readPin_A9);
  int_fast16_t value_A1 = adc->adc0->analogRead(readPin_A1);
  //uint16_t value_A2 = adc->adc1->analogRead(readPin_A2);
  ecg_diff = value_A9 - prev_value_ecg;
  prev_value_ecg = value_A9;
  pcg_diff = value_A1 - prev_value_pcg;
  prev_value_pcg = value_A1;
  // add rtc stuff here

  uint8_t msg [6]; 
  msg[0] = 0x55;
  msg[1] = (ecg_diff >> 8) & 255;
  msg[2] = ecg_diff & 255;
  msg[3] = (pcg_diff >> 8) & 255;
  msg[4] = pcg_diff & 255;;
  msg[5] = CRCFast(msg, 9);

  //logger->logEvent("Data Message Available");
  //logger->logEvent(msg,10);

  Serial.write(msg,6);
  Serial.flush();
}

void ReadADC(int_fast16_t &ecg_diff, uint_fast16_t &pcg_diff, time_t &sample_time){
  static int_fast16_t prev_value_ecg = 0;
  static int_fast16_t prev_value_pcg = 0;
  sample_time = Teensy3Clock.get();
  int_fast16_t value_A9 = adc->adc1->analogRead(readPin_A9);
  int_fast16_t value_A1 = adc->adc0->analogRead(readPin_A1);
  //uint16_t value_A2 = adc->adc1->analogRead(readPin_A2);
  ecg_diff = value_A9 - prev_value_ecg;
  prev_value_ecg = value_A9;
  pcg_diff = value_A1 - prev_value_pcg;
  prev_value_pcg = value_A1;
}

void ButtonInterrupt(){
  pwrToggle ^= 0x01;
  #if DEBUG == 2
  logger->logEvent("Forced Power Mode Change");
  #endif
}