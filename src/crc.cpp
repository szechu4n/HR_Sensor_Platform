#include "main.h"

void CRCInit(){
    crc  remainder;
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend << (WIDTH - 8);
        for (uint8_t bit = 8; bit > 0; --bit)
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

crc CRCFast(uint8_t const msg[], uint8_t nBytes){
    uint8_t data;
    crc remainder = 0;
    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = msg[byte] ^ (remainder >> (WIDTH - 8));
        remainder = CRCTable[data] ^ (remainder << 8);
    }
    return (remainder);
}