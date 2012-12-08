#include "../include/spi.h"

enum csPinEnable {
    CS_PIN_DISABLE = 0,
    CS_PIN_ENABLE  = 1,
};

static int32_t
SPI_init(const struct spiPort *self, uint32_t bitrate,
        SpiOpenFlags oFlags, enum csPinEnable useCsPin);

static int32_t
driveCsHigh(const struct spiPort *self);

static int32_t
driveCsLow(const struct spiPort *self);

static int32_t
spiTx (const struct spiPort *self, const void *data, size_t len);

static int32_t
spiRx (const struct spiPort *self, void *dest, size_t len);

static int32_t
spiTxStr (const struct spiPort *self, const char *str);

static const struct vtblSpiPort spiPortOps = {
    .tx             = &spiTx,
    .txStr          = &spiTxStr,
    .rx             = &spiRx,
    .driveCSHigh    = &driveCsHigh,
    .driveCSLow     = &driveCsLow,
};

int32_t
SPI_new(struct spiPort *self, SpiChannel chn, uint32_t bitrate,
        SpiOpenFlags oFlags)
{
    int32_t err;

    if (self == NULL)
        return -ENULPTR;

    self->op                = &spiPortOps;
    self->chn               = chn;
    self->autoCSPinEnable   = AUTO_CS_PIN_DISABLE;

    if ((err = SPI_init(self, bitrate, oFlags, CS_PIN_DISABLE)))
        return err;

    return 0;
}

int32_t
SPI_CS_new(struct spiPort *self, SpiChannel chn, uint32_t bitrate,
        SpiOpenFlags oFlags, enum autoCsPin autoCSPinEnable,
        IoPortId csPinLetter, uint32_t csPinNum)
{
    int32_t err;
    if (self == NULL)
        return -ENULPTR;

    self->op                = &spiPortOps;
    self->chn               = chn;
    self->autoCSPinEnable   = autoCSPinEnable;
    self->csPinLetter       = csPinLetter;
    self->csPinNum          = csPinNum;

    if ((err = SPI_init(self, bitrate, oFlags, CS_PIN_ENABLE)))
        return err;

    return 0;
}

static int32_t
SPI_init(const struct spiPort *self, uint32_t bitrate, SpiOpenFlags oFlags,
        enum csPinEnable useCsPin)
{

    uint32_t pbusFreqHz = sys_clk_hz/((uint32_t)(1 << OSCCONbits.PBDIV));

    if (self == NULL)
        return -ENULPTR;

    if (useCsPin == CS_PIN_ENABLE) {
        PORTSetPinsDigitalOut(self->csPinLetter, self->csPinNum);
        driveCsHigh(self);
    }

                                /* disable automatic driving of CS */
    SpiChnOpen(self->chn, oFlags & ~SPI_OPEN_MSSEN, pbusFreqHz/bitrate);

    return 0;
}

static int32_t
spiTx (const struct spiPort *self, const void *data, size_t len)
{
    int32_t err;
    uint32_t ui;
    const BYTE *dataBytes = (const BYTE *)data;

    if (self == NULL || data == NULL)
        return -ENULPTR;

    if (self->autoCSPinEnable == AUTO_CS_PIN_ENABLE)
        if ((err = driveCsLow(self))) {
            driveCsHigh(self);
            return err;
        }

    for (ui = 0; ui < len; ui++)
        SpiChnPutC(self->chn, dataBytes[ui]);

    delay_us(200);  /* must be at least 100us to work with Nokia 5110 LCD */
    if (self->autoCSPinEnable == AUTO_CS_PIN_ENABLE)
        if ((err = driveCsHigh(self)))
            return err;

    return 0;
}

static int32_t
spiTxStr (const struct spiPort *self, const char *str)
{
    int32_t err;

    if (self == NULL || str == NULL)
        return -ENULPTR;

    if ((err = spiTx(self, str, strlen(str))))
        return err;

    return 0;
}

static int32_t
spiRx (const struct spiPort *self, void *dest, size_t len)
{
    int32_t err;
    uint32_t ui;
    BYTE *dataBytes = (BYTE *)dest;

    if (self == NULL || dest == NULL)
        return -ENULPTR;

    if (self->autoCSPinEnable == AUTO_CS_PIN_ENABLE)
        if ((err = driveCsLow(self))) {
            driveCsHigh(self);
            return err;
        }

    /* Clear receive buffer */
    while (SpiChnRxBuffCount(self->chn))
        SpiChnReadC(self->chn);

    for (ui = 0; ui < len; ui++) {
        SpiChnPutC(self->chn, 0x00);
        SpiChnGetRov(self->chn, 1);
        dataBytes[ui] = (BYTE)SpiChnGetC(self->chn);
    }

    delay_us(20);
    if (self->autoCSPinEnable == AUTO_CS_PIN_ENABLE)
        if ((err = driveCsHigh(self)))
            return err;

    return 0;
}

static int32_t
driveCsHigh(const struct spiPort *self)
{
    if (self == NULL)
        return -ENULPTR;

    // delay_us(5);
    PORTSetBits(self->csPinLetter, self->csPinNum);
    delay_us(5);

    return 0;
}

static int32_t
driveCsLow(const struct spiPort *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTClearBits(self->csPinLetter, self->csPinNum);
    delay_us(5);

    return 0;
}
