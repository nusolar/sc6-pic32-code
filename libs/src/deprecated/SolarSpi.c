// for ltc6803
#include "../include/SolarSpi.h"
#include "../include/NUSPI.h"

const unsigned char crctab[] =
   {0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3};

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
unsigned long
crctablefast (const BYTE *p, size_t len)
{
    // fast lookup table algorithm without augmented zero bytes, e.g. used in pkzip.
    // only usable with polynom orders of 8, 16, 24 or 32.

    unsigned long crc = 0x41;

    while (len--)
        crc = (crc << 8) ^ crctab[ (crc & 0xff) ^ *p++];
                                                            // 8 here is referring to the polynomial order
    static const unsigned long crcmask = ((((unsigned long)1<<(8-1))-1)<<1)|1;

    crc&= crcmask;

    return crc;
}

BYTE
calcPEC (const BYTE *in, size_t len)
{
	return (BYTE)crctablefast(in, len);
}

BYTE
calcCmdPEC (BYTE cmd)
{
    return (BYTE)crctablefast(&cmd, 1);
}

void
BPSInit(SpiChannel chn, const BYTE cfgr0, const BYTE cfgr1, const BYTE cfgr2, const BYTE cfgr3, const BYTE cfgr4, const BYTE cfgr5)
{
	SPI_CS = 0;
    delay_us(1);

    BYTE cmd = WRCFG;
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));

    // buff[1] = calcPEC(buff, 1);
    // SpiChnPutS(chn, (unsigned int *)buff, 2); // Sends config code 0x01 and corresponding PEC code C7

    delay_us(10);

    /* byte array for top device, which is currently the only device */
    BYTE buff[7];
    buff[0] = cfgr0;
    buff[1] = cfgr1;
    buff[2] = cfgr2;
    buff[3] = cfgr3;
    buff[4] = cfgr4;
    buff[5] = cfgr5;
    buff[6] = calcPEC(buff, 6);
    //buff[6] = 0x01;
    int i;
    for (i = 0; i < 6; ++i)
    	SpiChnPutC(chn, buff[i]);

    /* then you would do the same for the middle device followed by the bottom device here */
    /* [code goes here] */

    // SpiChnPutS(chn, buff, 1); // Sends config codes for cfgr0-5 and corresponding PEC codes
    delay_us(20);
    SPI_CS = 1;
    delay_us(1);
}

void
BPSInits(SpiChannel chn, unsigned int numDevices, union BpsConfig *cfgs)
{
	SPI_CS = 0;
    delay_us(1);

    BYTE cmd = WRCFG;
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));

    delay_us(10);

    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        cfgs[i].pec = calcPEC(cfgs[i].data, sizeof(cfgs[0].data));
        unsigned int j;
        for (j = 0; j < sizeof(cfgs[0].byteArr); ++j)
            SpiChnPutC(chn, cfgs[i].byteArr[j]);
    }

    delay_us(20);
    SPI_CS = 1;
    delay_us(1);
}

void
ReadCfg(SpiChannel chn, unsigned int numDevices, union BpsConfig *cfgs)
{
    SPI_CS = 0;
    delay_us(1);
    BYTE cmd = RDCFG;   /* Read Config Register */
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));
    delay_us(15);

    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        unsigned int j;
        for (j = 0; j < sizeof(cfgs[0].byteArr); ++j) {
            SpiChnGetRov(chn, 1);
            SpiChnPutC(chn, 0x00);
            if (i == 0 && j == 0)
                SpiChnGetC(chn);
            cfgs[i].byteArr[j] = SpiChnGetC(chn);
            SpiChnGetRov(chn, 1);
        }
    }
    delay_us(20);
    SPI_CS = 1;
    delay_us(1);
}

void
ReadRawVoltages(SpiChannel chn, unsigned int numDevices, union RawVoltages *cvrs)
{
    SPI_CS = 0;
    delay_us(1);
    BYTE cmd = RDCV;
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));
    delay_us(15);

    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        unsigned int j;
        for (j = 0; j < sizeof(cvrs[0].byteArr); ++j) {
            SpiChnGetRov(chn, 1);
            SpiChnPutC(chn, 0x00);
            if (i == 0 && j == 0)
                SpiChnGetC(chn);
            cvrs[i].byteArr[j] = SpiChnGetC(chn);
            SpiChnGetRov(chn, 1);
        }
    }
    delay_us(20);
    SPI_CS = 1;
    delay_us(1);
}

void
ConvertVoltagesNew(unsigned int numDevices, union RawVoltages *cvrs, float *cellVoltages)
{
    unsigned int numVoltagePairs = sizeof(cvrs[0].voltagePair)/sizeof(RawCellVoltagePair);
    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        unsigned int j;
        for (j = 0; j < numVoltagePairs; ++j) {
            RawCellVoltagePair currentVPair = cvrs[i].voltagePair[j];
            cellVoltages[i*numVoltagePairs*2+j]     = (currentVPair.voltage1-512)*0.0015;
            cellVoltages[i*numVoltagePairs*2+j+1]   = (currentVPair.voltage2-512)*0.0015;
        }
    }
}

void
ConvertVoltages(unsigned int numDevices, union RawVoltages *cvrs, float *cellVoltages)
{
    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        unsigned int j;
        int currentRawVoltage;
        const unsigned int cellsPerDevice = 12;
        for (j = 0; j < cellsPerDevice; ++j) {
            /* The raw voltage register group uses three bytes to express two 12-bit raw voltage values
             * We will extract the raw voltage values 3 bytes at a time into a temporary int
             */
            if (!(j%2))
                memcpy(&currentRawVoltage, cvrs[i].data + j*3/2, 3);
            /* Bits 0-11 of temporary int contains the raw even-indexed cell voltages and bits 12-23
             * of the temporary contains the raw odd-indexed cell voltages. To obtain the actual cell
             * voltage, the equation needed is V_actual = (V_raw - 512)*1.5mV
             */
            cellVoltages[cellsPerDevice*i+j] = (((0xFFF) & (currentRawVoltage >> (j%2 ? 12 : 0)))-512)*0.0015;
        }
    }
}

void
SendCmd(SpiChannel chn, BYTE cmd)
{
    SPI_CS = 0;
    delay_us(1);
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));
    delay_ms(20);
    SPI_CS = 1;
    delay_us(1);
}

void
ReadDagn(SpiChannel chn, unsigned int numDevices, union Diagnostic *dagn)   /* Not working correctly: correct data is read for bottom device but not for top device */
{
    SPI_CS = 0;
    delay_us(1);
    BYTE cmd = DAGN;
    SpiChnPutC(chn, cmd);
    SpiChnPutC(chn, calcCmdPEC(cmd));
    delay_ms(20);
    SPI_CS = 1;
    delay_us(5);

    SPI_CS = 0;
    delay_us(1);
    BYTE cmd1 = RDDGNR;
    SpiChnPutC(chn, cmd1);
    SpiChnPutC(chn, calcCmdPEC(cmd1));
    delay_us(20);

    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        unsigned int j;
        for (j = 0; j < sizeof(dagn[0].byteArr); ++j) {
            SpiChnGetRov(chn, 1);
            SpiChnPutC(chn, 0x00);
            if (i == 0 && j == 0)
                SpiChnGetC(chn);
            dagn[i].byteArr[j] = SpiChnGetC(chn);
            SpiChnGetRov(chn, 1);
        }
    }
    delay_us(20);
    SPI_CS = 1;
    delay_us(1);
}

void
ConvertDagn(unsigned int numDevices, union Diagnostic *dagn, unsigned int *ref, unsigned int *muxfail, unsigned int *rev)
{
    unsigned int i;
    for (i = 0; i < numDevices; ++i) {
        int combinedDagn;
        memcpy(&combinedDagn, dagn[0].byteArr, sizeof(dagn[0].byteArr));
        ref[i] = (0xFFF) & combinedDagn;
        muxfail[i] = (0x01) & (combinedDagn >> 13);
        rev[i] = (0x03) & (combinedDagn >> 14);
    }
}
