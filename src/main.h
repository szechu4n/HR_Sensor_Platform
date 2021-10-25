#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include <avr/sleep.h>
#include "errorCodes.h"
#include "ErrorCheckLib.h"
#include "SystemCommands.h"
#include "crc.h"

uint8_t serialRxBuffer[1000];

enum MESSAGE_HEADER {
    SYNC,
    CMD,
    SAMPLE,
    CRC_ERROR
};
