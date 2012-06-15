#include "../include/onewire.h"

static const uint32_t owCrcTab[] = {
   0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65,
   157,195,33,127,252,162,64,30,95,1,227,189,62,96,130,220,
   35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98,
   190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255,
   70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7,
   219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154,
   101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36,
   248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185,
   140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205,
   17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80,
   175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238,
   50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115,
   202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139,
   87,9,235,181,54,104,138,212,149,203,41,119,244,170,72,22,
   233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168,
   116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53
};

/*
 * CRC Info:
 * X^8 + X^5 + X^4 + 1
 * order: 8
 * polynom (hex): 31 (found on http://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs)
 * initial value (hex): 00
 * final XOR value (hex): 00
 * YES reverse data bytes
 * YES reverse CRC result before final XOR
 */
inline uint32_t
owCrc(const void *data, size_t len)
{
    if (!data)
        return 0xFFFFFFFF;

    return crcTableFast(owCrcTab, data, len, 8, CRC_DIRECT, 0x00, CRC_8_DALLAS,
            CRC_REVERSE_DATA_BYTES, CRC_REVERSE_BEFORE_FINAL_XOR, 0x00);
}

static int32_t
reset(const struct oneWire *self);

static int32_t
tx(const struct oneWire *self, const void *data, size_t len);

static int32_t
txWithCrc(const struct oneWire *self, const void *data, size_t len);

static int32_t
rx(const struct oneWire *self, void *dst, size_t len);

static int32_t
rxCheckCrc(const struct oneWire *self, void *dst, size_t len);

static int32_t
driveLow(const struct oneWire *self);

static int32_t
driveHigh(const struct oneWire *self);

static int32_t
rxBit(const struct oneWire *self);

static int32_t
txBit(const struct oneWire *self, BIT b);

static int32_t
txByte(const struct oneWire *self, BYTE b);

static int32_t
first(struct oneWire *self, union romCode *dst, BYTE searchRomCmd);

static int32_t
findDevices(struct oneWire *self, union romCode *dst, size_t dstSizeBytes,
            BYTE searchRomCmd);

static int32_t
resetSearchState(struct oneWire *self);

static int32_t
verify(struct oneWire *self, union romCode rc, BYTE searchRomCmd);

static int32_t
findFamily(struct oneWire *self, union romCode *dst, size_t dstSizeBytes,
                    BYTE searchRomCmd, BYTE familyCode);

static int32_t
findSkipFamily(struct oneWire *self, union romCode *dst,
                        size_t dstSizeBytes, BYTE searchRomCmd,
                        BYTE familyCode);

static int32_t
powerBus(const struct oneWire *self);

static const struct vtblOneWire oneWireOps = {
    .powerBus           = &powerBus,
    .reset              = &reset,
    .tx                 = &tx,
    .txWithCrc          = &txWithCrc,
    .txBit              = &txBit,
    .txByte             = &txByte,
    .rx                 = &rx,
    .rxCheckCrc         = &rxCheckCrc,
    .rxBit              = &rxBit,
    .findDevices        = &findDevices,
    .findFamily         = &findFamily,
    .findSkipFamily     = &findSkipFamily,
    .verify             = &verify,
};

static int32_t
oneWire_init(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    /* Set LAT register, and then we'll manipulate TRIS register
     * via PORTSetPinsDigitalIn() and PORTSetPinsDigitalOut() to
     * control the bus, effectively toggling open-drain.
     */
    driveLow(self);

    return 0;
}

static int32_t
powerBus(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTSetBits(self->owPinLetter, self->owPinNum);
    PORTSetPinsDigitalOut(self->owPinLetter, self->owPinNum);
    PORTSetBits(self->owPinLetter, self->owPinNum); /* possibly not needed */

    return 0;
}

int32_t
oneWire_new(struct oneWire *self, IoPortId pinLtr, uint32_t pinNum)
{
    int32_t err = 0;

    if (self == NULL)
        return -ENULPTR;

    self->op            = &oneWireOps;
    self->owPinLetter   = pinLtr;
    self->owPinNum      = pinNum;
    if ((err = resetSearchState(self)) < 0)
        return err;
    memset(self->searchState.romCodeAndCrc.dataBytes, 0, sizeof(self->searchState.romCodeAndCrc.dataBytes));

    return oneWire_init(self);
}

static int32_t
driveLow(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTClearBits(self->owPinLetter, self->owPinNum);
    /* manipulate TRIS to pull to ground */
    PORTSetPinsDigitalOut(self->owPinLetter, self->owPinNum);

    return 0;
}

static int32_t
driveHigh(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTClearBits(self->owPinLetter, self->owPinNum);
    /* manipulate TRIS to make it high impedance */
    PORTSetPinsDigitalIn(self->owPinLetter, self->owPinNum);

    return 0;
}

static int32_t
readBit(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTSetPinsDigitalIn(self->owPinLetter, self->owPinNum);
    return (int32_t)PORTReadBits(self->owPinLetter, self->owPinNum);
}

static int32_t
reset(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    driveLow(self);
    delay_us(480);
    driveHigh(self);
    delay_us(70);
    BIT devicesPresent = !readBit(self);
    delay_us(410);
    driveHigh(self);

    return devicesPresent ? 1 : -ENODEV;
}

static int32_t
presenceDetected(const struct oneWire *self)
{
    int32_t err = 0;

    if (self == NULL)
        return -ENULPTR;

    if ((err = reset(self)) < 0)
        return err;

    return 1;
}

static int32_t
txBit(const struct oneWire *self, BIT b)
{
    if (self == NULL)
        return -ENULPTR;

    driveLow(self);
    if (b) {
        delay_us(6);
        driveHigh(self);
        delay_us(64);
    } else {
        delay_us(60);
        driveHigh(self);
        delay_us(10);
    }

    return 0;
}

static int32_t
tx(const struct oneWire *self, const void *data, size_t len)
{
    if (self == NULL || data == NULL)
        return -ENULPTR;

    const BYTE *dataBytes = (const BYTE *)data;
    uint32_t i, j;
    for (i = 0; i < len; i++)
        for (j = 0; j < 8; j++)
            txBit(self, (dataBytes[i]>>j)&1);

    return 0;
}

static int32_t
txByte(const struct oneWire *self, BYTE b)
{
    if (self == NULL)
        return -ENULPTR;

    return self->op->tx(self, &b, 1);
}

static int32_t
txWithCrc(const struct oneWire *self, const void *data, size_t len)
{
    int32_t err = 0;

    if (self == NULL || data == NULL)
        return -ENULPTR;

    if ((err = tx(self, data, len)) < 0)
        return err;

    BYTE crc = (BYTE)owCrc(data, len);
    if ((err = txByte(self, crc)) < 0)
        return err;

    return 0;
}

static int32_t
rxBit(const struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    driveLow(self);
    delay_us(6);
    driveHigh(self);
    BIT result = readBit(self);
    delay_us(55);
    
    return result;
}

static int32_t
rx(const struct oneWire *self, void *dst, size_t len)
{
    if (self == NULL || dst == NULL)
        return -ENULPTR;

    BYTE *dstBytes = (BYTE *)dst;
    memset(dstBytes, 0, len);

    unsigned int i, j;
    for (i = 0; i < len; i++)
        for (j = 0; j < 8; j++)
            dstBytes[i] |= (BYTE)(rxBit(self)<<j);

    return 0;
}

static int32_t
rxCheckCrc(const struct oneWire *self, void *dst, size_t len)
{
    int32_t err = 0;

    if (self == NULL || dst == NULL)
        return -ENULPTR;

    if ((err = rx(self, dst, len)) < 0)
        return err;

    BYTE crc = 0x00;
    if ((err = rx(self, &crc, sizeof(crc))) < 0)
        return err;

    if (crc != (BYTE)owCrc(dst, len))
        return -ECRC;

    return 0;
}

static int32_t
resetSearchState(struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    self->searchState.prevSearchWasLastDevice   = FALSE;
    self->searchState.lastDiscrepancyBit        = 0;
    self->searchState.lastFamilyDiscrepBit      = 0;

    return 0;
}

#define OW_NO_DEVS_LEFT_IN_SEARCH   12345

/**
 *
 * @param self
 * @param idBitNumber
 * @param lastZeroDiscrepBit
 * @return                      OW_NO_DEVS_LEFT_IN_SEARCH, error code, or
 *                                  the next bit (0 or 1)
 */
static int32_t
getNextBit(struct oneWire *self, uint32_t idBitNumber,
            uint32_t *lastZeroDiscrepBit)
{
    if (self == NULL || lastZeroDiscrepBit == NULL)
        return -ENULPTR;

    BIT idBit = self->op->rxBit(self);    /* all devices send the bit */
    BIT idBitComplement = self->op->rxBit(self);    /* ..then send the bit's
                                                     *  complement
                                                     */
    if (idBit == 1 && idBitComplement == 1) {       /* no devices left in
                                                     * search
                                                     */
        resetSearchState(self);
        return OW_NO_DEVS_LEFT_IN_SEARCH;
    }

    BIT searchDirection;
    if (idBit == 0 && idBitComplement == 0) {
        if (idBitNumber == self->searchState.lastDiscrepancyBit) {
            searchDirection = 1;
        } else if (idBitNumber > self->searchState.lastDiscrepancyBit) {
            searchDirection = 0;
        } else {
            /* search direction becomes idBitNumber bit in romCodeAndCrc */
            searchDirection =
                    self->searchState.romCodeAndCrc.dataBytes[(idBitNumber-1)/8] &
                    (1<<((idBitNumber-1)%8));
        }

        if (searchDirection == 0) {
            *lastZeroDiscrepBit = idBitNumber;

            if (*lastZeroDiscrepBit <= 8)
               self->searchState.lastFamilyDiscrepBit = *lastZeroDiscrepBit;
        }
    } else {
        searchDirection = idBit;
    }

    return searchDirection;
}

/**
 *
 * @param self
 * @param dst           The destination romCode variable.
 * @param searchRomCmd  The command byte to search for ROM codes. Most likely
 *                          either 0x0F or 0xEC.
 * @return              1 if a device was found, 0 or error code otherwise.
 */
static int32_t
searchRom(struct oneWire *self, union romCode *dst, BYTE searchRomCmd)
{
    int32_t err = 0;

    if (self == NULL)
        return -ENULPTR;

    if ((err = presenceDetected(self)) <= 0) { /* no presence detected or error */
        resetSearchState(self);
        return err;
    }
    
    if (self->searchState.prevSearchWasLastDevice == TRUE) {
        resetSearchState(self);
        return 0;
    }

    if ((err = txByte(self, searchRomCmd)))
        return err;

    uint32_t    idBitNumber          = 1;    /* 1 to 64 */
    uint32_t    lastZeroDiscrepBit   = 0;    /* bit posn of last zero written
                                              * where there was a discrepancy
                                              * (1-64, unless there was no
                                              * discrepancy in which case it
                                              * remains 0).
                                              */

    while (idBitNumber <= 64) {
        int32_t getNxtBitResult = getNextBit(self, idBitNumber,
                &lastZeroDiscrepBit);
        if (getNxtBitResult == OW_NO_DEVS_LEFT_IN_SEARCH) {
            resetSearchState(self);
            return 0;
        }

        if (getNxtBitResult < 0)    /* error */
            return getNxtBitResult;

        BIT nxtBit = (BIT)getNxtBitResult;

        if (nxtBit)
            BIT_SET(self->searchState.romCodeAndCrc.dataBytes[(idBitNumber-1)/8],
                    (idBitNumber-1)%8);
        else
            BIT_CLEAR(self->searchState.romCodeAndCrc.dataBytes[(idBitNumber-1)/8],
                    (idBitNumber-1)%8);

        self->op->txBit(self, nxtBit);

        idBitNumber++;
    }

    self->searchState.lastDiscrepancyBit = lastZeroDiscrepBit;
    if (self->searchState.lastDiscrepancyBit == 0)
        /* UNCHANGED - no discrepancies occured */
        self->searchState.prevSearchWasLastDevice = TRUE;

    BYTE crc = self->searchState.romCodeAndCrc.crc;
    if (dst != NULL)
        *dst = self->searchState.romCodeAndCrc.rc;

    if (crc != (BYTE)owCrc(&(self->searchState.romCodeAndCrc.rc),
                                sizeof(self->searchState.romCodeAndCrc.rc)))
        return -ECRC;

    return 1;
}

static int32_t
first(struct oneWire *self, union romCode *dst, BYTE searchRomCmd)
{
    int32_t err = 0;

    if (self == NULL)
        return -ENULPTR;

    if ((err = resetSearchState(self)) < 0)
        return err;
    
    return searchRom(self, dst, searchRomCmd);
}

static int32_t
next(struct oneWire *self, union romCode *dst, BYTE searchRomCmd)
{
    if (self == NULL)
        return -ENULPTR;

    return searchRom(self, dst, searchRomCmd);
}

/**
 *
 * @param self
 * @param dst
 * @param dstSizeBytes
 * @param searchRomCmd
 * @return              Number of devices found or negative error code.
 */
static int32_t
findDevices(struct oneWire *self, union romCode *dst, size_t dstSizeBytes,
            BYTE searchRomCmd)
{
    int32_t rc = 0;
    uint32_t arraySize = dstSizeBytes/sizeof(*dst);

    if (self == NULL || (dst == NULL && dstSizeBytes != 0))
        return -ENULPTR;

    uint32_t i;
    for (i = 0; i < INT_MAX; i++) {
        union romCode *rcPtr = NULL;
        if (i < arraySize)
            rcPtr = &dst[i];

        if (i == 0) {
            if ((rc = first(self, rcPtr, searchRomCmd)) != 1)
                return rc;
        } else {
            rc = next(self, rcPtr, searchRomCmd);
            if (rc == 0)
                break;
            else if (rc < 0)    /* error */
                return rc;
        }
    }

    return (int32_t)i;
}

/**
 *
 * @param self
 * @param rc
 * @param searchRomCmd
 * @return              1 if specified romCode 'rc' is found, 0 or error code
 *                          otherwise.
 */
static int32_t
verify(struct oneWire *self, union romCode rc, BYTE searchRomCmd)
{
    int32_t err = 0;
    union romCode searchRomResultRc;

    if (self == NULL)
        return -ENULPTR;

    self->searchState.romCodeAndCrc.rc          = rc;
    self->searchState.romCodeAndCrc.crc         = owCrc(&rc, sizeof(rc));

    self->searchState.lastDiscrepancyBit        = 64;
    self->searchState.lastFamilyDiscrepBit      = 0;
    self->searchState.prevSearchWasLastDevice   = FALSE;

    if ((err = searchRom(self, &searchRomResultRc, searchRomCmd) <= 0))
        return err;

    if (!memcmp(&rc, &searchRomResultRc, sizeof(rc)))
        return 1;

    return 0;
}

static int32_t
targetSetup(struct oneWire *self, BYTE familyCode)
{
    if (self == NULL)
        return -ENULPTR;

    self->searchState.lastDiscrepancyBit            = 64;
    self->searchState.lastFamilyDiscrepBit          = 0;
    self->searchState.prevSearchWasLastDevice       = FALSE;
    self->searchState.romCodeAndCrc.rc.familyCode   = familyCode;
    memset(self->searchState.romCodeAndCrc.rc.serialNum, 0,
            sizeof(self->searchState.romCodeAndCrc.rc.serialNum));

    return 0;
}

static int32_t
findFamily(struct oneWire *self, union romCode *dst, size_t dstSizeBytes,
                    BYTE searchRomCmd, BYTE familyCode)
{
    uint32_t ui;
    uint32_t arraySize = dstSizeBytes/sizeof(*dst);

    if (self == NULL || (dst == NULL && dstSizeBytes != 0))
        return -ENULPTR;

    targetSetup(self, familyCode);

    for (ui = 0; ui < INT_MAX; ui++) {
        union romCode currentRomCode;
        int32_t nxtResult = next(self, &currentRomCode, searchRomCmd);
        if (nxtResult == 0 || currentRomCode.familyCode != familyCode)
            break;
        else if (nxtResult < 0)    /* error */
            return nxtResult;
        else if (ui < arraySize)
            dst[ui] = currentRomCode;
    }

    return (int32_t)ui;
}

static int32_t
familySkipSetup(struct oneWire *self)
{
    if (self == NULL)
        return -ENULPTR;

    self->searchState.lastDiscrepancyBit =
            self->searchState.lastFamilyDiscrepBit;
    self->searchState.lastFamilyDiscrepBit = 0;
    self->searchState.prevSearchWasLastDevice = FALSE;

    return 0;
}

static int32_t
findSkipFamily(struct oneWire *self, union romCode *dst,
                        size_t dstSizeBytes, BYTE searchRomCmd,
                        BYTE familyCode)
{
    union romCode dummyRomCode;

    if (self == NULL || (dst == NULL && dstSizeBytes != 0))
        return -ENULPTR;

    resetSearchState(self);

    targetSetup(self, familyCode);

    memset(&dummyRomCode, 0, sizeof(dummyRomCode));
    next(self, &dummyRomCode, searchRomCmd);

    if (dummyRomCode.familyCode == familyCode) {
        familySkipSetup(self);
    } else {
        resetSearchState(self);
    }

    uint32_t i;
    uint32_t dstArraySize = dstSizeBytes/sizeof(*dst);
    for (i = 0; i < INT_MAX; i++) {
        union romCode currentRomCode;
        int32_t nxtResult = next(self, &currentRomCode, searchRomCmd);
        if (nxtResult == 0 || currentRomCode.familyCode == familyCode)
            break;
        else if (nxtResult < 0)    /* error */
            return nxtResult;
        else if (i < dstArraySize)
            dst[i] = currentRomCode;
    }

    return (int32_t)i;
}
