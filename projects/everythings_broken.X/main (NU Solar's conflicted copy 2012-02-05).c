#include <plib.h>

#include "../libs/include/NU32v2.h"
#include "../libs/include/Ltc6803_new.h"
#include "../libs/include/NUSPI_new.h"
#include "../libs/include/Nokia5110_new.h"
#include "../libs/include/NUCAN_new.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

const unsigned int SYS_CLK_HZ = 80000000;

int main(int argc, char** argv)
{
    initNU32(115200);
    
    struct nokia5110 display;
    struct nokia5110 *dp = nokia5110_init(&display, SPI_CHANNEL3,
                            IOPORT_E, BIT_2, IOPORT_E, BIT_1, IOPORT_E, BIT_0);

    union BpsConfig cfg[2];
    memset(&cfg, 0, sizeof(cfg));
    cfg[0].cfgr0 = WDT | GPIO1 | GPIO2 | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(4.3);
    cfg[0].vuv = convertUVLimit(2.75);

    cfg[1] = cfg[0];

    /*
    cfg.cfgr0 = 0xE1;
    cfg.cfgr1 = 0x00;
    cfg.cfgr2 = 0x00;
    cfg.cfgr3 = 0x00;
    cfg.cfgr4 = 0x9C;
    cfg.cfgr5 = 0xC7;
     */
    
    struct ltc6803 bps;
    struct ltc6803 *bpsp = &bps;
    ltc6803_new(bpsp, SPI_CHANNEL4, IOPORT_G, BIT_9, 2, cfg);

    /*
    PORTSetPinsDigitalOut(IOPORT_G,BIT_9);

    struct spiPort adc;
    struct spiPort *adcp = &adc;
    SPI_CS_new(adcp, SPI_CHANNEL4, TRUE, 1000000, SYS_FREQ,
                SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON, IOPORT_G, BIT_9);
     */
     
    while (1) {
        static int i = 0;
        i++;

        dp->op->clear(dp);
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "fun time! %d", i);

        /*
        PORTSetBits(IOPORT_G,BIT_9);
        delay_us(10);
        PORTClearBits(IOPORT_G,BIT_9);

        BYTE data[2];
        adcp->op->rx(adcp,&data,sizeof(data));
        INT16 newData = data[0]<<8 | data[1];

        double voltage = 5 * (double) newData / (1<<15);
        dp->op->gotoXY(dp, 0, 1);
        dp->op->printf(dp, "%X %X", data[0], data[1]);
        dp->op->gotoXY(dp, 0, 4);
        dp->op->printf(dp, "%f", voltage);

        double current = (voltage-2.5)/12.5E-3;
        dp->op->gotoXY(dp, 0, 5);
        dp->op->printf(dp, "%f", current);
         */

        ltc6803_new(bpsp, SPI_CHANNEL4, IOPORT_G, BIT_9, 2, cfg);
        if (!bpsp->op->cfgsMatch(bpsp, cfg)) {
            dp->op->gotoXY(dp, 0,2);
            dp->op->printf(dp, "HOORAY!");
        }

        delay_ms(100);
    }

    return (EXIT_SUCCESS);
}
