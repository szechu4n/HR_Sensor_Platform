#include "main.h"

uint8_t serialMsg[6];
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
  // #elif DEBUG == 2
  // Serial.println("System Fully Initialized");
  // logger->logEvent("System Fully Initialized");
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
        for(int i = 0; i < 6; i++)
        {
          serialMsg[i++] = Serial.read();
        }
        #if DEBUG == 2
        logger->logEvent(serialMsg,i);
        #endif
        CmdInvoker(serialMsg);
      }
      #if DEBUG == 1
      logger->logEvent("Message Received");
      logger->logEvent(serialMsg, i);
      // #elif DEBUG == 2
      // Serial.println("Message Received");
      // logger->logEvent("Message Received");
      // logger->logEvent(serialMsg, i);
      #endif
      // ErrorSet(CRCCheck(serialMsg, i)); // i = len
      // CmdInvoker(serialMsg);
    }
    else{
      digitalWrite(ledSerialPin_D7, HIGH);
    }
    // #if DEBUG == 2
    // Serial.println("Going to sleep mode");
    // #endif
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
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);

  adc->adc1->setAveraging(16);
  adc->adc1->setResolution(16);
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
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
  static uint16_t pcg_sample;
  static time_t sample_time;
  ReadADC(ecg_diff, pcg_sample, sample_time);
  
  #if DEBUG == 1
  logger->logEvent("Data Sampled");
  #endif
  
  uint8_t ecg[2];
  uint8_t pcg[2];
  uint8_t time[4];

  ecg[0] = ecg_diff & 255;
  ecg[1] = (ecg_diff >> 8) & 255;

  pcg[0] = pcg_sample & 255;
  pcg[1] = (pcg_sample >> 8) & 255;

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
  #if DEBUG == 1
  logger->logEvent("Data Message Available");
  logger->logEvent(msg,10);
  #endif
  Serial.write(msg,10);
}

void ReadADC(int16_t &ecg_diff, uint16_t &pcg, time_t &sample_time){
  static uint16_t prev_value_ecg = 0;
  sample_time = Teensy3Clock.get();
  uint16_t value_A9 = adc->adc1->analogRead(readPin_A9);
  uint16_t value_A1 = adc->adc0->analogRead(readPin_A1);
  uint16_t value_A2 = adc->adc1->analogRead(readPin_A2);
  ecg_diff = value_A9 - prev_value_ecg;
  prev_value_ecg = value_A9;
  pcg = (value_A1 + value_A2)/2;
}

void ButtonInterrupt(){
  pwrToggle ^= 0x01;
  #if DEBUG == 2
  logger->logEvent("Forced Power Mode Change");
  #endif
}