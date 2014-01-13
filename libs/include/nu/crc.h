/*
 * CRC calculation functions based on code from
 * http://www.zorc.breitbandkatze.de/crctester.c.
 * The main page (http://www.zorc.breitbandkatze.de/crc.html) has a useful CRC
 * calculator.
 */

#ifndef NU_CRC_H
#define NU_CRC_H 1

#include <GenericTypeDefs.h>
#include "nu/errorcodes.h"

#ifdef __cplusplus
extern "C" {
#endif


/* from http://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs */
#define CRC_1           0x01                /* x+1 */
#define CRC_PARITY      CRC_1               /* parity bit calculation */
#define CRC_4_ITU       0x03                /* x^4+x+1 */
#define CRC_5_EPC       0x09                /* x^5+x^3+1 */
#define CRC_5_ITU       0x15                /* x^5+x^4+x^2+1 */
#define CRC_5_USB       0x05                /* x^5+x^2+1 */
#define CRC_6_ITU       0x03                /* x^6+x+1 */
#define CRC_7           0x09                /* x^7+x^3+1 */
#define CRC_8_CCITT     0x07                /* x^8+x^2+x+1 */
#define CRC_8_DALLAS    0x31                /* x^8+x^5+x^4+1 */
#define CRC_8_MAXIM     CRC_8_DALLAS
#define CRC_8_1_WIRE    CRC_8_DALLAS
#define CRC_1_WIRE      CRC_8_DALLAS
#define CRC_8           0xD5                /* x^8+x^7+x^6+x^4+x^2+1 */
#define CRC_8_SAE_J1850 0x1D
#define CRC_8_WCDMA     0x9B
#define CRC_10          0x233
#define CRC_11          0x385
#define CRC_12          0x80F
#define CRC_15_CAN      0x4599              /* x^15+x^14+x^10+x^8+x^7+x^4+x^3+1 */
#define CRC_CAN         CRC_15_CAN
#define CRC_16_IBM      0x8005
#define CRC_16          CRC_16_IBM
#define CRC_16_CCITT    0x1021
#define CRC_CCITT       CRC_16_CCITT
#define CRC_16_T10_DIF  0x8BB7
#define CRC_16_DNP      0x3D65
#define CRC_16_DECT     0x0589
#define CRC_16_ARINC    0xA02B
/* XMODEM polynom from http://www.zorc.breitbandkatze.de/crc.html */
#define CRC_16_XMODEM   0x8408
#define CRC_XMODEM      CRC_16_XMODEM
#define CRC_24          0x5D6D6B
#define CRC_24_RADIX_64 0x864CFB
#define CRC_30          0x2030B9C7
#define CRC_32          0x04C11DB7
#define CRC_32C         0x1EDC6F41
#define CRC_32K         0x741B8CD7
#define CRC_32Q         0x814141AB
#define CRC_40_GSM      0x0004820009
#define CRC_64_ISO      0x000000000000001B
#define CRC_64_ECMA_182 0x42F0E1EBA9EA3693

enum direct {
    CRC_NONDIRECT,
    CRC_DIRECT
};

enum reverseDataBytes {
    CRC_NO_REVERSE_DATA_BYTES,
    CRC_REVERSE_DATA_BYTES
};

/* reverse crc result before final xor? */
enum reverseBeforeFinalXor {
    CRC_NO_REVERSE_BEFORE_FINAL_XOR,
    CRC_REVERSE_BEFORE_FINAL_XOR
};

int
generateCrcTable(unsigned long *crcTabDst, enum reverseDataBytes refIn,
        unsigned int order, unsigned long polynom);

unsigned long
crcTableFast (const uint32_t *crcTab, const void *data, size_t len,
        unsigned int order, enum direct direct, unsigned long initVal,
        unsigned long polynom, enum reverseDataBytes reverseDataBytes,
        enum reverseBeforeFinalXor, unsigned long final_xor_value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
