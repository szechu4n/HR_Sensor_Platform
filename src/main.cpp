#include "main.h"

uint8_t serialRxBuffer[1000];
uint8_t pwrToggle;

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
  #elif DEBUG == 2
  Serial.println("System Fully Initialized");
  logger->logEvent("System Fully Initialized");
  #endif
  ErrorSet(PowerOnSelfTest());
  while(1){
    if(pwrToggle == 0x01)
    {
      digitalWrite(ledSerialPin_D7, LOW);
    }
    else if(Serial.available()){
      digitalWrite(ledSerialPin_D7, HIGH);
      int i = 0;
      while(Serial.available()){
        serialRxBuffer[i++] = Serial.read();
      }
      #if DEBUG == 1
      logger->logEvent("Message Received");
      logger->logEvent(serialRxBuffer, i);
      #elif DEBUG == 2
      Serial.println("Message Received");
      logger->logEvent("Message Received");
      logger->logEvent(serialRxBuffer, i);
      #endif
      ErrorSet(CRCCheck(serialRxBuffer, i)); // i = len
      CmdInvoker(serialRxBuffer);
    }
    else{
      digitalWrite(ledSerialPin_D7, HIGH);
    }
    SleepModeIdle();
  }
  return 0;
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
  static int16_t ecg_diff;
  static uint16_t pcg1_diff, pcg2_diff;
  static time_t sample_time;
  ReadADC(ecg_diff, pcg1_diff, pcg2_diff, sample_time);
  
  uint8_t* ecg_diff_ptr  = (uint8_t*)&ecg_diff; // really worried about this causing seg fault lol
  uint8_t* pcg1_diff_ptr = (uint8_t*)&pcg1_diff;
  uint8_t* pcg2_diff_ptr = (uint8_t*)&pcg2_diff;
  uint8_t* time_ptr = (uint8_t*)&sample_time;
  // add rtc stuff here

  uint8_t msg [12]; 
  msg[0] = 0x55;
  msg[1] = time_ptr[3];
  msg[2] = time_ptr[2];
  msg[3] = time_ptr[1];
  msg[4] = time_ptr[0];
  msg[5] = ecg_diff_ptr[1];
  msg[6] = ecg_diff_ptr[0];
  msg[7] = pcg1_diff_ptr[1];
  msg[8] = pcg1_diff_ptr[0];
  msg[9] = pcg2_diff_ptr[1];
  msg[10] = pcg2_diff_ptr[0];
  msg[11] = CRCFast(msg, 11);
  #if DEBUG == 1
  logger->logEvent("Data Message Available");
  logger->logEvent(msg,12);
  #endif
  Serial.write(msg,16);  
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

void ButtonInterrupt(){
  pwrToggle ^= 0x01;
}