#include "ds18x20.h"
#include "nu32.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct ds18x20 ds; struct ds18x20 *dsp = &ds;
    union romCode rcs[2] = {0};
    uint32_t ui;
    int32_t err;

    nu32_init(80000000);
    CloseTimer4();
    delay(1);

    nu32_init_leds();

    /* for some reason 1w seems to not work on a lot of pins...
     * It seems to for sure work on RA0, RB0, RD0, RE0, RF0, and RG0
     */
    err = ds18x20_new(dsp, IOPORT_A, BIT_0, PARASITIC_POWER_DISABLE);

    err = dsp->op->findDevices(dsp, rcs, sizeof(rcs));

    for (ui = 0; ui < ARRAY_SIZE(rcs); ++ui) {
        union romCode rc = rcs[ui];
        err = dsp->op->verify(dsp, rcs[ui]);
        rc = rc;
    }

    dsp->op->startTempConversion(dsp, ALL_DEVICES);

    delay(1);

    for (ui = 0; ui < ARRAY_SIZE(rcs); ++ui) {
        double temp = dsp->op->readTemp(dsp, rcs[ui]);
        Nop();
        temp = temp;
    }

    while (1)
        ; /* do nothing */

    return 0;
}