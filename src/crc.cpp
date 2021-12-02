#include "main.h"

uint_fast8_t CRCTable[256];

void CRCInit(){
    uint_fast8_t remainder;
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend << (WIDTH - 8);
        for (uint_fast8_t bit = 8; bit > 0; --bit)
        {
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
        CRCTable[dividend] = remainder;
    }
}

uint8_t CRCFast(uint8_t msg[], uint_fast8_t nBytes){
    uint_fast8_t data;
    uint_fast8_t remainder = 0;
    for (uint_fast8_t byte = 0; byte < nBytes; ++byte)
    {
        data = msg[byte] ^ (remainder >> (WIDTH - 8));
        remainder = CRCTable[data] ^ (remainder << 8);
    }
    return uint8_t(remainder);
}

ErrorCode CRCCheck(uint_fast8_t msg[], uint_fast8_t nBytes){
    return NO_ERROR;
}