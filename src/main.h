#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "ADC.h"
#include <avr/sleep.h>
#include "errorCodes.h"
#include "ErrorCheckLib.h"
#include "SystemCommands.h"
#include "crc.h"

const char VER_MAJOR = 'A';
const char VER_MINOR = 'A';

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
ErrorCode PowerOnSelfTest();
void SleepModeIdle();
void ReadADC(int16_t &, uint16_t &, uint16_t &, time_t &);
void WriteToSDCard();
void ProcessSystem();

#endif