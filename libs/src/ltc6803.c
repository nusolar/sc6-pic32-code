#include "../include/ltc6803.h"

enum ltc6803Cmds {
    WRITECFGS   = 0x01,
    RDCFGS,
    RDCV        = 0x04,
    RDCVA       = 0x06,
    RDCVB       = 0x08,
    RDCVC       = 0x0A,
    RDFLG       = 0x0C,
    RDTMP       = 0x0E,
    STCVAD      = 0x10,
    STCVAD_1,
    STCVAD_2,
    STCVAD_3,
    STCVAD_4,
    STCVAD_5,
    STCVAD_6,
    STCVAD_7,
    STCVAD_8,
    STCVAD_9,
    STCVAD_10,
    STCVAD_11,
    STCVAD_12,
    STCVAD_CLR,
    STCVAD_SELFTEST1,
    STCVAD_SELFTEST2,
    STOWAD      = 0x20,
    STOWAD_1,
    STOWAD_2,
    STOWAD_3,
    STOWAD_4,
    STOWAD_5,
    STOWAD_6,
    STOWAD_7,
    STOWAD_8,
    STOWAD_9,
    STOWAD_10,
    STOWAD_11,
    STOWAD_12,
    STTMPAD     = 0x30,
    STTMPAD_EXT1,
    STTMPAD_EXT2,
    STTMPAD_INTERNAL,
    STTMPAD_SELFTEST1,
    STTMPAD_SELFTEST2,
    POLLADC     = 0x40,
    POLLINT     = 0x50,
    DAGN        = 0x52,
    READ_DAGN   = 0x54,
    STCV_DISCHRG    = 0x60,
    STCV_DISCHRG1,
    STCV_DISCHRG2,
    STCV_DISCHRG3,
    STCV_DISCHRG4,
    STCV_DISCHRG5,
    STCV_DISCHRG6,
    STCV_DISCHRG7,
    STCV_DISCHRG8,
    STCV_DISCHRG9,
    STCV_DISCHRG10,
    STCV_DISCHRG11,
    STCV_DISCHRG12,
    STOW_DISCHRG    = 0x70,
    STOW_DISCHRG1,
    STOW_DISCHRG2,
    STOW_DISCHRG3,
    STOW_DISCHRG4,
    STOW_DISCHRG5,
    STOW_DISCHRG6,
    STOW_DISCHRG7,
    STOW_DISCHRG8,
    STOW_DISCHRG9,
    STOW_DISCHRG10,
    STOW_DISCHRG11,
    STOW_DISCHRG12,
};

static const unsigned char crctab[] =
   {0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
    0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
    0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
    0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
    0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
    0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
    0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
    0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
    0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
    0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
    0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
    0xFA, 0xFD, 0xF4, 0xF3};

union Diagnostic {
    struct {
        BYTE dgnr0;
        BYTE dgnr1;
    };
    struct {
        unsigned ref        :12;
        unsigned unused     :1;
        unsigned muxfail    :1;
        unsigned rev        :2;
    } __attribute__ ((__packed__));
    BYTE byteArr[2];
};

typedef union {
    struct {
        unsigned val :24;
    } __attribute__((__packed__));
    struct {
        unsigned voltage1 :12;
        unsigned voltage2 :12;
    } __attribute__((__packed__));
    struct {
        BYTE lb;
        BYTE mb;
        BYTE hb;
    };
} RawCellVoltagePair;

union RawVoltages {
    union {
        BYTE data[18];
        RawCellVoltagePair voltagePair[6];
    };
    BYTE byteArr[18];
};

static float
convertRefToVolts(int ref);

static int32_t
writeCfgs (struct ltc6803 *self, const union BpsConfig *cfgs);

static int32_t
readCfgs (struct ltc6803 *self, union BpsConfig *cfgs);

static int32_t
checkCfgsMatch (struct ltc6803 *self, const union BpsConfig *cfgs);

static int32_t
sendWithPec (struct ltc6803 *self, const void *data, size_t len);

static int32_t
sendCmdAndPec (struct ltc6803 *self, enum ltc6803Cmds cmd);

static int32_t
rxDataCheckPecs (struct ltc6803 *self, void *data, size_t len);

static int32_t
startVoltageConversion (struct ltc6803 *self);

static int32_t
startOpenWireConversion (struct ltc6803 *self);

static int32_t
convertVoltages(struct ltc6803 *self, union RawVoltages *cvrs, float *cellVoltages);

static int32_t
readVolts (struct ltc6803 *self, float *voltages);

static int32_t
readRawVolts (struct ltc6803 *self, union RawVoltages *rxRv);

static int32_t
post (struct ltc6803 *self);

static int32_t
checkOpenWire(struct ltc6803 *self, uint16_t *openWire, const float *voltages);

static const struct vtblLtc6803 ltc6803Ops = {
    .writeCfgs                  = &writeCfgs,
    .cfgsMatch                  = &checkCfgsMatch,
    .startVoltageConversion     = &startVoltageConversion,
    .startOpenWireConversion    = &startOpenWireConversion,
    .readVolts                  = &readVolts,
    .post                       = &post,
    .checkOpenWire              = &checkOpenWire,
};

/*
 * Based on code found at http://zorc.breitbandkatze.de/crc.html (scroll down on page)
 *
 * From LTC6803 data sheet:
 * characteristic polynomial x^8 + x^2 + x + 1
 * initial PEC value of 01000001 (0x41)
 *
 * Therefore, CRC calculator settings are as follows:
 * CRC order (1..64)    : 8
 * CRC polynom (hex)    : 07
 * Initial value (hex)  : 41 (direct)
 * Final XOR value (hex): 00
 * DON'T reverse data bytes
 * DON'T reverse CRC result before Final XOR
 */
static long long
crcTableFast (const void *data, size_t len)
{
    if (data == NULL)
        return -ENULPTR;

    const BYTE *dataBytes = (const BYTE *)data;

    unsigned long crc = 0x41;

    while (len--)
        crc = (crc << 8) ^ crctab[ (crc & 0xff) ^ *dataBytes++];
                                                               /* 8 here is referring to the polynomial order */
    /* static const unsigned long crcmask = ((((unsigned long)1<<(8-1))-1)<<1)|1; */
    unsigned long crcmask = 0xff;

    crc &= crcmask;

    return crc;
}

static int32_t
ltc6803_init (struct ltc6803 *self, const union BpsConfig *cfgs);

int32_t
ltc6803_new (struct ltc6803 *self, SpiChannel chn,
        IoPortId csPinLtr,          uint32_t csPinNum,
        uint32_t numDevices,    const union BpsConfig *cfgs)
{
    int32_t err;

    if (!self || !cfgs)
        return -ENULPTR;

    self->op                    = &ltc6803Ops;
    self->numDevices            = numDevices;
    self->flags_mismatchCfgs    = 0;
    self->flags_mismatchPecs    = 0;

    if ((err = SPI_CS_new(&(self->spiPort), chn, 100000,
                    SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON,
                    AUTO_CS_PIN_DISABLE, csPinLtr, csPinNum)) < 0)
        return err;

    if ((err = ltc6803_init(self, cfgs)) < 0)
        return err;

    return 0;
}

static int32_t
ltc6803_init (struct ltc6803 *self, const union BpsConfig *cfgs)
{
    int32_t err;
    uint32_t retryCount;

    if (!self || !cfgs)
        return -ENULPTR;

    if ((err = self->spiPort.op->driveCSHigh(&(self->spiPort))) < 0)
        return err;

    for (retryCount = 0; retryCount < 3; retryCount++)
        /* @TODO checkCfgsMatch() is redundant as it is now called by writeCfgs() */
        if (!((err = writeCfgs(self, cfgs)) < 0) &&
                !((err = checkCfgsMatch(self, cfgs)) < 0))
            break;

    if (err < 0)
        return err;

    return 0;
}

static int
transactionCmd (struct ltc6803 *self, enum ltc6803Cmds cmd)
{
    int err;

    if (self == NULL)
        return -ENULPTR;

    if ((err = self->spiPort.op->driveCSLow(&(self->spiPort))))
        return err;
    if ((err = sendCmdAndPec(self, cmd))) {
        self->spiPort.op->driveCSHigh(&(self->spiPort));
        return err;
    }
    if ((err = self->spiPort.op->driveCSHigh(&(self->spiPort))))
        return err;

    return 0;
}

/* dst[0] is bottom device */
static int
transactionCmdRx (struct ltc6803 *self, enum ltc6803Cmds cmd, void *dst, size_t len)
{
    int32_t err = 0;

    if (self == NULL || dst == NULL)
        return -ENULPTR;

    if ((err = self->spiPort.op->driveCSLow(&(self->spiPort))) < 0) {
        return err;
    } else if ((err = sendCmdAndPec(self, cmd)) < 0) {
        self->spiPort.op->driveCSHigh(&(self->spiPort));
        return err;
    } else if ((err = rxDataCheckPecs(self, dst, len)) < 0) {
        self->spiPort.op->driveCSHigh(&(self->spiPort));
        return err;
    } else if ((err = self->spiPort.op->driveCSHigh(&(self->spiPort))) < 0) {
        return err;
    }

    return 0;
}

/* data[0] is bottom device */
static int
transactionCmdTx (struct ltc6803 *self, enum ltc6803Cmds cmd, const void *data, size_t len)
{
    int err;

    if (self == NULL || data == NULL)
        return -ENULPTR;

    if ((err = self->spiPort.op->driveCSLow(&(self->spiPort))))
        return err;
    if ((err = sendCmdAndPec(self, cmd))) {
        self->spiPort.op->driveCSHigh(&(self->spiPort));
        return err;
    }

    const BYTE *dataBytes = (const BYTE *)data;
    size_t sizeOneElem = len/(self->numDevices);
    long i;
    for (i = self->numDevices - 1; i >= 0; --i)
        if ((err = sendWithPec(self, &(dataBytes[(unsigned long)i*sizeOneElem]), sizeOneElem)) < 0) {
            self->spiPort.op->driveCSHigh(&(self->spiPort));
            return err;
        }

    if ((err = self->spiPort.op->driveCSHigh(&(self->spiPort))))
        return err;

    return 0;
}

inline BYTE
convertUVLimit (double vuv)
{
    return (BYTE) ((vuv/(16*.0015)) + 31);
}

inline BYTE
convertOVLimit (double vov)
{
    return (BYTE) ((vov/(16*.0015)) + 32);
}

static float
convertRefToVolts (int ref)
{
    return (((double)ref - 512) * .0015);
}

/* writes are top to bottom */
static int
writeCfgs (struct ltc6803 *self, const union BpsConfig *cfgs)
{
    int err;

    if (self == NULL || cfgs == NULL)
        return -ENULPTR;

    if ((err = transactionCmdTx(self, WRITECFGS, cfgs, sizeof(*cfgs))))
        return err;

    return checkCfgsMatch(self, cfgs);
}

static double
convertVoltage(unsigned int rawV)
{
    return ((double)rawV-512)*0.0015;
}

static int
convertVoltages(struct ltc6803 *self, union RawVoltages *cvrs, float *cellVoltages)
{
    if (self == NULL || cvrs == NULL  || cellVoltages == NULL)
        return -ENULPTR;

    unsigned int voltagePairsPerDevice = ARRAY_SIZE((*cvrs).voltagePair);
    unsigned int cellsPerDevice = voltagePairsPerDevice*2;
    unsigned int i;
    for (i = 0; i < self->numDevices; ++i) {
        unsigned int j;
        for (j = 0; j < voltagePairsPerDevice; ++j) {
            RawCellVoltagePair currentVPair = cvrs[i].voltagePair[j];
            cellVoltages[i*cellsPerDevice+j*2]      = convertVoltage(currentVPair.voltage1);
            cellVoltages[i*cellsPerDevice+j*2+1]    = convertVoltage(currentVPair.voltage2);
        }
    }
    return 0;
}

static int32_t
startVoltageConversion (struct ltc6803 *self)
{
    if (self == NULL)
        return -ENULPTR;

    return transactionCmd(self, STCVAD);
}

static int32_t
startOpenWireConversion (struct ltc6803 *self)
{
    if (self == NULL)
        return -ENULPTR;

    return transactionCmd(self, STOWAD);
}

static int
readDagn (struct ltc6803 *self, union Diagnostic *dagn)
{
    int err;

    if (self == NULL || dagn == NULL)
        return -ENULPTR;

    if ((err = transactionCmd(self, DAGN)))
        return err;

    delay_ms(25);

    return transactionCmdRx(self, READ_DAGN, dagn, sizeof(*dagn) * self->numDevices);
}


static int
readVolts (struct ltc6803 *self, float *voltages)
{
    int err;
    union RawVoltages rxRv[(self->numDevices)];

    if ((err = readRawVolts(self, rxRv)))
        return err;

    return convertVoltages(self, rxRv, voltages);
}

static int
readRawVolts (struct ltc6803 *self, union RawVoltages *rxRv)
{
    if (self == NULL || rxRv == NULL)
        return -ENULPTR;

    return transactionCmdRx(self, RDCV, rxRv, sizeof(*rxRv) * self->numDevices);
}

/* reads are top to bottom as well USING THIS FUNCTION */
static int
readCfgs (struct ltc6803 *self, union BpsConfig *cfgs)
{
    if (self == NULL || cfgs == NULL)
        return -ENULPTR;

    return transactionCmdRx(self, RDCFGS, cfgs, sizeof(*cfgs) * self->numDevices);
}

static int
rxDataCheckPecs (struct ltc6803 *self, void *data, size_t len)
{
    int err = 0, pecResult = 0;

    if (self == NULL || data == NULL)
        return -ENULPTR;

    size_t sizeOneElement = len/(self->numDevices);

    BYTE rcvBuff[sizeOneElement+1];   /* add 1 for the pec */
    BYTE *dataBytes = (BYTE *)data;

    self->flags_mismatchPecs = 0;
    unsigned int i;
    for (i = 0; i < self->numDevices; ++i) {
        err |= self->spiPort.op->rx(&(self->spiPort), rcvBuff, sizeof(rcvBuff));
        memcpy(&dataBytes[i*sizeOneElement], rcvBuff, sizeOneElement);

        BYTE pec = rcvBuff[sizeof(rcvBuff)-1];

        int crc = (int) crcTableFast(&dataBytes[i*sizeOneElement], sizeOneElement);
        if (crc < 0 && !err)    /* error, but only record if we don't already have an error */
            err = crc;

        if (pec != (BYTE) crc) {
            pecResult = -ECRC;
            self->flags_mismatchPecs |= (unsigned int)(1<<i);
        }
    }

    if (pecResult)
        return pecResult;

    if (err)
        return err;

    return 0;
}

static int
checkCfgsMatch (struct ltc6803 *self, const union BpsConfig *cfgs)
{
    int err;

    if (self == NULL || cfgs == NULL)
        return -ENULPTR;

    union BpsConfig rxCfgs[self->numDevices];
    int retryCount;
    for (retryCount = 0; retryCount < 3; retryCount++)
        if (!(err = readCfgs(self, rxCfgs)))
            break;

    if (err) {
        self->flags_mismatchCfgs = self->flags_mismatchPecs;
        return err;
    }

    self->flags_mismatchCfgs = 0;
    unsigned int i;
    for (i = 0; i < self->numDevices; i++) {
        if (memcmp(&cfgs[i], &rxCfgs[i], sizeof(*rxCfgs))) {
            self->flags_mismatchCfgs |=  (unsigned int)(1<<i);

            err = -ELTC6803CFG;

            union BpsConfig zeroCfg;
            memset(&zeroCfg, 0, sizeof(zeroCfg));
            if (!memcmp(rxCfgs[i].byteArr, zeroCfg.byteArr, sizeof(*rxCfgs)) &&
                    memcmp(cfgs[i].byteArr, zeroCfg.byteArr, sizeof(*cfgs))) {
                err = -ENODEV;
            }
        }
    }

    if (err)
        return err;

    return 0;
}

static int
sendCmdAndPec (struct ltc6803 *self, enum ltc6803Cmds cmd)
{
    int err;

    if (self == NULL)
        return -ENULPTR;

    if ((err = sendWithPec(self, &cmd, 1)))
        return err;

    return 0;
}

static int
sendWithPec (struct ltc6803 *self, const void *data, size_t len)
{
    int err;

    if (self == NULL || data == NULL)
        return -ENULPTR;

    long pec = (long)crcTableFast(data, len);
    if (pec < 0)    /* error */
        return (int) pec;

    if ((err = self->spiPort.op->tx(&(self->spiPort), data, len)))
        return err;

    if ((err = self->spiPort.op->tx(&(self->spiPort), &pec, 1)))
        return err;

    return 0;
}

static int
post (struct ltc6803 *self)
{
    int err = 0;
    if (self == NULL)
        return -ENULPTR;

    if ((err = transactionCmd(self, STCVAD_SELFTEST1)))
        return err;

    delay_ms(25);

    union RawVoltages rxRv[(self->numDevices)];
    memset(rxRv, 0, sizeof(rxRv));
    if ((err = readRawVolts(self, rxRv)))
        return err;

    union RawVoltages expected[(self->numDevices)];
    memset(expected, 0x55, sizeof(expected));
    if (memcmp(rxRv, expected, sizeof(rxRv)))
        return -ELTC6803ADC;

    union Diagnostic dagn[self->numDevices];
    if ((err = readDagn(self, dagn)))
        return err;

    unsigned int i;
    for (i = 0; i < self->numDevices; ++i) {
        if (dagn[i].muxfail == 1)
            return -ELTC6803MUX;
        if (convertRefToVolts(dagn[i].ref) < 2.1 || convertRefToVolts(dagn[i].ref) > 2.9)
            return -ELTC6803REF;
    }

    return 0;
}

static int32_t
checkOpenWire(struct ltc6803 *self, uint16_t *openWire, const float *voltages)
{
    if (self == NULL || openWire == NULL || voltages == NULL)
        return -ENULPTR;

    uint32_t i;
    for (i = 0; i < self->numDevices; i++) {
        uint32_t j;
        for(j = 0; j < 12; j++) {
            if (voltages[i+j] < 0.5)
                openWire[i] |= 1<<j;
        }
    }

    return 0;
}
