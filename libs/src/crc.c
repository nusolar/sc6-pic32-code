#include "../include/crc.h"

static inline unsigned long
crcMask (unsigned int order)
{
    return (((1UL<<(order-1))-1)<<1)|1;
}

static inline unsigned long
crcHighBit (unsigned int order)
{
    return 1UL<<(order-1);
}

static unsigned long
reflect(unsigned long crc, unsigned int bitnum)
{
    unsigned long i, j=1, crcout=0;
    for (i=1UL<<(bitnum-1); i; i>>=1) {
        if (crc & i)
            crcout |= j;
        j<<=1;
    }
    return crcout;
}

static unsigned long
directToNondirect(unsigned long initVal_direct, unsigned int order, unsigned long polynom)
{
    unsigned long crc = initVal_direct;
    unsigned long highBit = crcHighBit(order);
    unsigned int i;
    for (i = 0; i < order; ++i) {
        unsigned long _bit = crc & highBit;
        if (_bit)
            crc ^= polynom;
        crc >>= 1;
        if (_bit)
            crc |= highBit;
    }
    
    return crc;
}

static unsigned long
nondirectToDirect (unsigned long initVal_nondirect, unsigned int order, unsigned long polynom)
{
    unsigned long crc = initVal_nondirect;
    unsigned long highBit = crcHighBit(order);
    unsigned int i;
    for (i = 0; i < order; ++i) {
        unsigned long _bit = crc & highBit;
        crc <<= 1;
        if (_bit)
            crc ^= polynom;
    }

    crc &= crcMask(order);
    return crc;
}

int
generateCrcTable(unsigned long *crcTabDst, enum reverseDataBytes refIn,
        unsigned int order, unsigned long polynom) {
    unsigned long highBit, mask, ui;

    if (crcTabDst == NULL)
        return -ENULPTR;

    if (order < 8)
        return -EINVAL;

    highBit = crcHighBit(order);
    mask = crcMask(order);

    for (ui = 0; ui < 256; ++ui) {
        unsigned int uj;
        unsigned long crc = ui;
        
        if (refIn == CRC_REVERSE_DATA_BYTES)
            crc = reflect(crc, 8);
        crc<<=(order-8);

        for (uj = 0; uj < 8; ++uj) {
            unsigned long _bit = crc & highBit;
            crc<<=1;
            if (_bit)
                crc ^= polynom;
        }

        if (refIn)
            crc = reflect(crc, order);

        crc &= mask;
        crcTabDst[ui] = crc;
    }

    return 0;
}

unsigned long
crcTableFast (const unsigned long *crcTab, const void *data, size_t len,
        unsigned int order, enum direct direct, unsigned long initVal,
        unsigned long polynom, enum reverseDataBytes refIn,
        enum reverseBeforeFinalXor refOut, unsigned long final_xor_value)
{
    unsigned long crc;
    const BYTE *dataBytes = (const BYTE *)data;

    if (!crcTab || !data)
        return ~0UL;

    /* only usable with orders 8, 16, 24, and 32 */
    if (order < 1 || order > 32 || order%8)
        return ~0UL;

    if (direct == CRC_DIRECT)
        crc = initVal;
    else
        crc = nondirectToDirect(initVal, order, polynom);

    if (refIn == CRC_REVERSE_DATA_BYTES)
        crc = reflect(crc, order);

    if (refIn == CRC_NO_REVERSE_DATA_BYTES)
        while (len--)
            crc = (crc<<8) ^ crcTab[((crc>>(order-8)) & 0xff) ^ *dataBytes++];
    else
        while (len--)
            crc = (crc>>8) ^ crcTab[(crc & 0xff) ^ *dataBytes++];

    if ((refOut == CRC_REVERSE_BEFORE_FINAL_XOR) ^
            (refIn == CRC_REVERSE_DATA_BYTES))
        crc = reflect(crc, order);

    crc ^= final_xor_value;
    crc &= crcMask(order);

    return crc;
}

long long
crcTable (const unsigned long *crcTab, const void *data, size_t len,
        unsigned int order, enum direct direct, unsigned long initVal,
        unsigned long polynom, enum reverseDataBytes refIn,
        enum reverseBeforeFinalXor refOut, unsigned long final_xor_value)
{
    unsigned long crc;
    const BYTE *dataBytes = (const BYTE *)data;

    if (!crcTab || !data)
        return -ENULPTR;

    /* only usable with orders 8, 16, 24, and 32 */
    if (order < 1 || order > 32 || order%8)
        return -EINVAL;

    if (direct == CRC_NONDIRECT)
        crc = initVal;
    else
        crc = directToNondirect(initVal, order, polynom);

    if (refIn == CRC_REVERSE_DATA_BYTES)
        crc = reflect(crc, order);

    if (refIn == CRC_REVERSE_DATA_BYTES)
        while (len--)
            crc = ((crc>>8) | (*dataBytes++ << (order-8))) ^ crcTab[crc & 0xff];
    else
        while (len--)
            crc = ((crc<<8) | *dataBytes++) ^ crcTab[(crc>>(order-8)) & 0xff];

    if (refIn == CRC_REVERSE_DATA_BYTES)
        while (++len < order/8)
            crc = (crc>>8)^crcTab[crc & 0xff];
    else
        while (++len < order/8)
            crc = (crc<<8) ^ crcTab[(crc>>(order-8)) & 0xff];

    if ((refOut == CRC_REVERSE_BEFORE_FINAL_XOR) ^
            (refIn == CRC_REVERSE_DATA_BYTES))
        crc = reflect(crc, order);

    crc ^= final_xor_value;
    crc &= crcMask(order);

    return crc;
}

/* NOT DONE */
long long
crcBitByBit (const void *data, size_t len,
        unsigned int order, enum direct direct, unsigned long initVal,
        unsigned long polynom, enum reverseDataBytes refIn,
        enum reverseBeforeFinalXor refOut, unsigned long final_xor_value)
{
    unsigned long ui, uj, currentBit;
    const BYTE *dataBytes = (const BYTE *)data;

    unsigned long crc;
    if (direct == CRC_DIRECT)
        crc = directToNondirect(initVal, order, polynom);
    else
        crc = initVal;

    for (ui = 0; ui < len; ui++) {
        unsigned long c = *dataBytes++;
        if (refIn == CRC_REVERSE_DATA_BYTES)
            c = reflect(c, 8);
        
    }

    return 0;
}
