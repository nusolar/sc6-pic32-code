#include <stdint.h>
#include <stdlib.h>

#define SYS_CLK_HZ  (80000000L)    /* 80 MHz */

#include "../libs/include/error_reporting.h"
#include "../libs/include/nokia5110.h"
#include "../libs/include/nu32.h"
#include "../libs/include/ds18x20.h"
#include "../libs/include/onewire.h"

/* config pragmas needed mostly for PICKit3 */
#pragma config ICESEL   = ICS_PGx2
#pragma config FPLLMUL  = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV   = DIV_1
#pragma config POSCMOD  = XT, FNOSC = PRIPLL

/* disable WDT (we'll control it in software) */
#pragma config FWDTEN   = OFF

/* 2.048s WDT timeout (1:2048 postscaler) */
#pragma config WDTPS    = PS2048

int32_t
main(void)
{
    nu32_init(115200);

    struct nokia5110 display;
    struct nokia5110 *dp = &display;
    nokia5110_new(dp, SPI_CHANNEL3,
                            IOPORT_E, BIT_2, IOPORT_E, BIT_1, IOPORT_E, BIT_0);
    dp->op->clear(dp);
    dp->op->gotoXY(dp, 0, 0);
    dp->op->printf(dp, "working");

    register_reporting_nokia5110(dp, REP_INFO);
    reportf(__FILE__, __LINE__, REP_WARNING, -1, "HALLO THAR!");
    report_err(REP_WARNING, -5, "halp");

    Nop();

    struct ds18x20 ds;
    struct ds18x20 *dsp = &ds;
    ds18x20_new(dsp, IOPORT_D, BIT_0, PARASITIC_POWER_ENABLE);

    union romCode rcs[3];

    int64_t result = dsp->op->readRom(dsp, &rcs[0]);
    uint64_t rc = 0;
    memcpy(&rc, &rcs[0], sizeof(*rcs));
    Nop();

    result = dsp->op->findDevices(dsp, rcs, sizeof(rcs));
    uint64_t rc0 = 0, rc1 = 0;
    memcpy(&rc0, &rcs[0], sizeof(*rcs));
    memcpy(&rc1, &rcs[1], sizeof(*rcs));

    struct ds18b20ScratchPad dsscratch = {0};
    result = dsp->op->readScratch(dsp, &dsscratch, rcs[0]);
    int64_t scratch = 0;
    memcpy(&scratch, &dsscratch, sizeof(dsscratch));

    dsp->op->startTempConversion(dsp, ALL_DEVICES);
    delay_ms(750);

    double temp = dsp->op->readTemp(dsp, rcs[0]);
    Nop();

    /*
    struct oneWire ow;
    struct oneWire *owp = &ow;
    oneWire_new(owp, IOPORT_D, BIT_0);

    struct nokia5110 n;
    struct nokia5110 *np = &n;
    nokia5110_new(np, SPI_CHANNEL3, IOPORT_E, BIT_2, IOPORT_E, BIT_1,
                                IOPORT_E, BIT_0);

    led1_on();
    delay(2);
    led1_off();

    uint32_t count = 0;
    while(1) {
        np->op->clear(np);
        np->op->gotoXY(np, 0, 0);
        np->op->printf(np, "%d", count);

        union romCode rcs[3];

        int32_t result = owp->op->findFamily(owp, rcs, sizeof(rcs), 0xF0, 0x28);
        int64_t rc1 = 0, rc2 = 0;
        memcpy(&rc1, &rcs[0], 7);
        memcpy(&rc2, &rcs[1], 7);

        union romCode tmp = {
            .byteArr = {0x28, 0x75, 0xA1, 0x4E, 0x03, 0x00, 0x00},
        };
        tmp.familyCode = 0x28;

        result = owp->op->verify(owp, tmp, 0xF0);    
        Nop();

        if (result > 0) {
            np->op->gotoXY(np, 0, 1);
            np->op->printf(np, "HALLO, THAR!");
            led1_on();
            delay(1);
            led1_off();
        }

        count++;
        led0_toggle();
    }
     */

    exit(EXIT_SUCCESS);
}
