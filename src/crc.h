#ifndef CRC_H
#define CRC_H

typedef uint8_t crc;
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8

void CRCInit();
crc CRCFast(uint8_t[], uint8_t);
ErrorCode CRCCheck(uint8_t[], uint8_t);

#endif