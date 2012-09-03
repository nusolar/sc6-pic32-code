#include "../include/spi_new.h"

int32_t
SPI_new(struct spiPort *self, csControlFunc csControl, spiSendByteFunc spiSendByte, spiInitFunc spiInit)
{
    int32_t err;
    
    if (!self)
        return -ENULPTR;
    
    self->op            = &spiPortOps;
    self->csControl     = csControl;
    self->spiSendByte   = spiSendByte;
    
    if ((err = spiInit(self)) < 0)
        return err;
    
    return 0;
}

static int32_t
spiTx(const struct spiPort *self, const void *data, size_t len)
{
    int32_t err;
    uint32_t ui;
    const uint8_t *dataBytes = (const uint8_t *)data;
    
    if (!self || !data)
        return -ENULPTR;
    
    self->csControl(0);
    for (ui = 0; ui < len; ++ui)
        self->spiSendByte(self, dataBytes[ui]);
    self->csControl(1);
    
    delay_us(200);
}
