#include "ds18x20.h"
#include "nokia5110.h"
#include "nu32.h"

#include "common_pragmas.h"

static const SpiChannel    NOKIA_SPI_CHANNEL       = SPI_CHANNEL2;
static const IoPortId      NOKIA_DC_PIN_LTR        = IOPORT_E;
static const uint32_t      NOKIA_DC_PIN_NUM        = BIT_9;
/* nokia1 */
static const IoPortId      NOKIA1_CS_PIN_LTR       = IOPORT_G;
static const uint32_t      NOKIA1_CS_PIN_NUM       = BIT_9;
static const IoPortId      NOKIA1_RESET_PIN_LTR    = IOPORT_A;
static const uint32_t      NOKIA1_RESET_PIN_NUM    = BIT_9;

static const IoPortId      DS18B20_PIN_LTR         = IOPORT_A;
static const uint32_t      DS18B20_PIN_NUM         = BIT_0;

static union romCode rcs[100] = {0};

static struct ds18x20 ds, *dsp = &ds;
static struct nokia5110    display1,   *dp1         = &display1;

int32_t
main(void)
{
    uint32_t ui;

    nu32_init(80000000);
    ds18x20_new(dsp, DS18B20_PIN_LTR, DS18B20_PIN_NUM,
                    PARASITIC_POWER_DISABLE);
    nokia5110_new(dp1, NOKIA_SPI_CHANNEL,
                                NOKIA1_CS_PIN_LTR, NOKIA1_CS_PIN_NUM,
                                NOKIA1_RESET_PIN_LTR, NOKIA1_RESET_PIN_NUM,
                                NOKIA_DC_PIN_LTR, NOKIA_DC_PIN_NUM);

    while (1) {
        int32_t numDevices = dsp->op->findDevices(dsp, rcs, sizeof(rcs));
        float minTemp = 9999;
        float maxTemp = 0;
        union romCode *maxRc = NULL;
        dsp->op->startTempConversion(dsp, ALL_DEVICES);
        delay(.75);
        for (ui = 0; numDevices > 0 && ui < numDevices; ui++) {
            float temp = dsp->op->readTemp(dsp, rcs[ui]);
            if (temp > maxTemp) {
                maxTemp = temp;
                maxRc = rcs+ui;
            }
            if (temp < minTemp)
                minTemp = temp;
        }
        dp1->op->clear(dp1);
        dp1->op->gotoXY(dp1, 0, 0);
        dp1->op->printf(dp1, "devices:%d", numDevices);
        dp1->op->gotoXY(dp1, 0, 1);
        dp1->op->printf(dp1, "min:%f", minTemp);
        dp1->op->gotoXY(dp1, 0, 2);
        dp1->op->printf(dp1, "max:%f", maxTemp);
        dp1->op->gotoXY(dp1, 0, 3);
        dp1->op->printf(dp1, "%02X %02X %02X %02X %02X %02X %02X",
            maxRc->byteArr[0], maxRc->byteArr[1], maxRc->byteArr[2], maxRc->byteArr[3], maxRc->byteArr[4], maxRc->byteArr[5], maxRc->byteArr[6]);
    }

    return 0;
}
