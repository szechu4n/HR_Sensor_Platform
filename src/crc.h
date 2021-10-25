typedef uint8_t crc;
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8

crc CRCTable[256];

void CRCInit();
crc CRCFast();
ErrorCode CRCCheck(uint8_t[], uint8_t);