#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include <avr/sleep.h>
#include "errorCodes.h"
#include "ErrorCheckLib.h"
#include "SystemCommands.h"
#include "crc.h"

uint8_t serialRxBuffer[1000]; // may not be necessary after all

enum MESSAGE_HEADER { // may need to remove
    SYNC,
    CMD,
    SAMPLE,
    CRC_ERROR
};
