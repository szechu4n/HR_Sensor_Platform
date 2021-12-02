#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "SPI.h"
#include "SDFat.h"
#include "ADC.h"
#include <avr/sleep.h>
#include "errorCodes.h"
#include "ErrorCheckLib.h"
#include "SystemCommands.h"
#include "crc.h"
#include "logger.h"

#define DEBUG 1

const char VER_MAJOR = '0';
const char VER_MINOR = '1';
const char VER_DEBUG = '1';

const int ledErrorPin_D8 = 8;
const int ledSerialPin_D7 = 7;
const int buttonStartPin_D12 = 12;

const int readPin_A1 = A1;
const int readPin_A2 = A2;
const int readPin_A9 = A9;

//extern uint8_t serialRxBuffer[1000]; // may not be necessary after all

enum MESSAGE_HEADER { // may need to remove
    SYNC,
    CMD,
    SAMPLE,
    CRC_ERROR
};

void ADCInit();
void SerialInit();
void PlatformInit();
void ButtonInit();
ErrorCode PowerOnSelfTest();
void SleepModeIdle();
void ReadADC(int_fast16_t &, uint_fast16_t &, time_t &);
void WriteToSDCard();
void ProcessSystem();
void ProcessSystemFast();
void ButtonInterrupt();


#endif