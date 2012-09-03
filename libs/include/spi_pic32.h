#ifndef NU_SPI_PIC32_H
#define NU_SPI_PIC32_H

#include "spi_new.h"

struct spi_pic32 {
    struct spiPort  spi;
    uint32_t        csPinNum;
    IoPortId        csPinLtr;
    SpiChannel      chn;
};

int32_t
SPI_pic32_new(struct spiPort *self, uint32_t csPinNum, IoPortId csPinLtr, SpiChannel chn);

#endif
