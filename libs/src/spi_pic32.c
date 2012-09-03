#include "spi_pic32.h"

#define SPI_TO_PIC32SPI(x)  ...

int32_t
SPI_pic32_new(struct spiPort *self, uint32_t csPinNum, IoPortId csPinLtr, SpiChannel chn)
{
    self->csPinNum  = csPinNum;
    self->csPinltr  = csPinLtr;
    self->chn       = chn;
    
    SPI_new(self, &csControlFunc, &spiSendByte, &spi_init);
}

static int32_t
spi_init(const struct spiPort *self)
{
    uint32_t pbusFreqHz = sys_clk_hz/((uint32_t)(1 << OSCCONbits.PBDIV));
    if (!self)
        return -ENULPTR;
    
    SpiChnOpen(...)
}

static int32_t
csControlFunc(const struct spiPort *self, uint32_t driveHigh)
{
    if (!self)
        return -ENULPTR;
    
    const struct spi_pic32 *s = SPI_TO_PIC32SPI(self);
    
    if (driveHigh)
        PORTSetBits(s->csPinLtr, s->csPinNum);
    else
        PORTClearBits(s->csPinLtr, s->csPinNum);
    
    return 0;
}

static int32_t
spiSendByte(const struct spiPort *self, uint8_t byte)
{
    if (!self)
        return -ENULPTR;
    
    const struct spi_pic32 *s = SPI_TO_PIC32SPI(self);
    SpiChnPutC(s->chn, byte);
    
    return 0;
}
