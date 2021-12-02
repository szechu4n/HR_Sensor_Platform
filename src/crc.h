#ifndef CRC_H
#define CRC_H

#define WIDTH  (8 * sizeof(uint_fast8_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8

void CRCInit();
uint8_t CRCFast(uint8_t[], uint_fast8_t);
ErrorCode CRCCheck(uint_fast8_t[], uint_fast8_t);

#endif