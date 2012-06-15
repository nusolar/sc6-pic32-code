#include "../include/NUSPI.h"

inline void
initSPI(SpiChannel chn)
{
    SPI_CS_TRIS = 0; /* set as output */
    SPI_CS = 1;
    SpiChnOpen(chn,
        SPI_OPEN_MSTEN|SPI_OPEN_CKE_REV|
        SPI_OPEN_MODE8,SYS_FREQ/DESIRED_BITRATE);
}

void
NUSPITx (SpiChannel chn, const BYTE *data, size_t len)
{
    SPI_CS = 0;
    delay_us(1);
    unsigned int i;
    for (i = 0; i < len; i++)
    {
        SpiChnPutC(chn, data[i]);
    }
    delay_us(20);
    SPI_CS = 1;
}

void
NUSPIRx(SpiChannel chn, BYTE *data, size_t len)
{
    SPI_CS = 0;
    delay_us(1);
    unsigned int i;
    for (i = 0; i < len; i++) {
        SpiChnGetRov(chn, 1);
        SpiChnPutC(chn, 0x00);
        if (i == 0)
            SpiChnGetC(chn);
        data[i] = (BYTE)SpiChnGetC(chn);
        SpiChnGetRov(chn, 1);
    }
    delay_us(20);
    SPI_CS = 1;
}

