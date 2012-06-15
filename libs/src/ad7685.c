#include "../include/ad7685.h"

static int32_t
readVolts(struct ad7685 *self, float *voltages);

static int32_t
convertAndReadVolts(struct ad7685 *self, float *voltages);

static int32_t
driveCnvLow(struct ad7685 *self);

static int32_t
driveCnvHigh(struct ad7685 *self);

const struct vtbl_ad7685 ad7685_ops = {
    .convertAndReadVolts = &convertAndReadVolts,
};

static int32_t
ad7685_init(struct ad7685 *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTSetPinsDigitalOut(self->cnvPinLtr, self->cnvPinNum);

    return 0;
}

#ifndef NEXAMPLE
static void
example_ad7685_new(void)
{
    /** [ad7685_new example] */
    struct ad7685 adc;
    struct ad7685 *adcp = &adc;
    /* setup 1 ad7685 device on SPI_CHANNEL3 with convert pin set to G8 */
    ad7685_new(adcp, SPI_CHANNEL3, IOPORT_G, BIT_8, 1, THREE_WIRE,
                USE_BUSY_INDICATOR);
    /** [ad7685_new example] */
}
#endif

int32_t
ad7685_new(struct ad7685 *self, SpiChannel chn, IoPortId cnvPinLtr, uint32_t cnvPinNum, uint32_t numDevices,
            enum WireConfiguration WireConfig, enum BusyIndicator UseBusyIndicator)
{
    if (((WireConfig == FOUR_WIRE && UseBusyIndicator == NO_BUSY_INDICATOR) ||
            (WireConfig == CHAIN_MODE && UseBusyIndicator == USE_BUSY_INDICATOR)))
        return -EINVAL;
    
    int32_t err;

    if (self == NULL)
        return -ENULPTR;

    self->op                = &ad7685_ops;
    self->WireConfig        = WireConfig;
    self->UseBusyIndicator  = UseBusyIndicator;
    self->numDevices        = numDevices;
    self->cnvPinLtr         = cnvPinLtr;
    self->cnvPinNum         = cnvPinNum;

    if ((err = SPI_new(&(self->spiPort), chn, 100000,
            SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON)))
        return err;

    if ((err = ad7685_init(self)))
        return err;

    return 0;
}

#ifndef NEXAMPLE
static void
example_ad7685_CS_new(void)
{
    /** [ad7685_CS_new example] */
    struct ad7685 adc;
    struct ad7685 *adcp = &adc;
    /* setup 2 ad7685 devices on SPI_CHANNEL4 with convert pin set to G9 and
     *  chip-select pin set to G8
     */
    ad7685_CS_new(adcp, SPI_CHANNEL4, IOPORT_G, BIT_9, 2, FOUR_WIRE,
                USE_BUSY_INDICATOR, IOPORT_G, BIT_8);
    /** [ad7685_CS_new example] */
}
#endif

int32_t
ad7685_CS_new(struct ad7685 *self, SpiChannel chn, IoPortId cnvPinLtr, uint32_t cnvPinNum, uint32_t numDevices,
            enum WireConfiguration WireConfig, enum BusyIndicator UseBusyIndicator,
            IoPortId csPinLtr, uint32_t csPinNum)
{
    int32_t err;

    if (self == NULL)
        return -ENULPTR;

    self->op                = &ad7685_ops;
    self->WireConfig        = WireConfig;
    self->UseBusyIndicator  = UseBusyIndicator;
    self->numDevices        = numDevices;
    self->cnvPinLtr         = cnvPinLtr;
    self->cnvPinNum         = cnvPinNum;

    if ((err = SPI_CS_new(&(self->spiPort), chn, 100000,
                SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON,
                AUTO_CS_PIN_DISABLE, csPinLtr, csPinNum)))
        return err;

    if ((err = ad7685_init(self)))
        return err;

    return 0;
}

#ifndef NEXAMPLE
static void
example_convertAndReadVolts(void)
{
    /** [convertAndReadVolts example] */
    struct ad7685 adc;
    struct ad7685 *adcp = &adc;
    /* setup 2 ad7685 devices on SPI_CHANNEL4 with the convert pin set to G9 */
    ad7685_new(adcp, SPI_CHANNEL4, IOPORT_G, BIT_9, 2, THREE_WIRE,
                USE_BUSY_INDICATOR);
    float voltages[2];
    adcp->op->convertAndReadVolts(adcp, voltages);
    /** [convertAndReadVolts example] */
}
#endif

/* gets the actual voltage reading(s) (not raw data) */
static int32_t
convertAndReadVolts(struct ad7685 *self, float *voltages)
{
    int32_t err;

    if (self == NULL || voltages == NULL)
        return -ENULPTR;

    if (self->WireConfig == FOUR_WIRE &&
            self->UseBusyIndicator == USE_BUSY_INDICATOR)
        if ((err = self->spiPort.op->driveCSHigh(&(self->spiPort))))
            return err;

    /* start conversion */
    if ((err = driveCnvHigh(self)))
        return err;

    delay_us(.1);

    if (self->WireConfig == THREE_WIRE &&
            self->UseBusyIndicator == USE_BUSY_INDICATOR)
        if ((err = driveCnvLow(self)))
            return err;

    if (self->WireConfig == FOUR_WIRE &&
            self->UseBusyIndicator == USE_BUSY_INDICATOR)
        if ((err = self->spiPort.op->driveCSLow(&(self->spiPort)))) {
            driveCnvLow(self);
            return err;
        }

    delay_us(2.3);

    if (self->WireConfig == THREE_WIRE &&
            self->UseBusyIndicator == NO_BUSY_INDICATOR)
        if ((err = driveCnvLow(self)))
            return err;

    /* read in the actual voltage reading(s) over SPI */
    readVolts(self, voltages);

    driveCnvLow(self);
    delay_us(5);

    return 0;
}

static int32_t
readVolts(struct ad7685 *self, float *voltages)
{
    int32_t err;

    if (self == NULL || voltages == NULL)
        return -ENULPTR;

    uint16_t data[self->numDevices];
    if ((err = self->spiPort.op->rx(&(self->spiPort), &data, sizeof(data))))
        return err;

    uint32_t i;
    for (i = 0; i < self->numDevices; ++i) {
        /* swap byte order ... */
        data[i] = (uint16_t) (((data[i] >> 8) & 0xFF) |
                                    ((data[i] << 8) & 0xFF00));
        /* then compute the actual voltage */
        voltages[i] = (5 * ((float) data[i])) / (1 << 16);
    }
    
    return 0;
}

static int32_t
driveCnvHigh(struct ad7685 *self) {
    if (self == NULL)
        return -ENULPTR;

    PORTSetBits(self->cnvPinLtr, self->cnvPinNum);

    return 0;
}

static int32_t
driveCnvLow(struct ad7685 *self) {
    if (self == NULL)
        return -ENULPTR;

    PORTClearBits(self->cnvPinLtr, self->cnvPinNum);
    
    return 0;
}
