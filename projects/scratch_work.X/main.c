/* 
 * File:   main.c
 * Author: akahn
 *
 * Created on February 25, 2012, 4:36 PM
 */

#include <plib.h>

#include "../libs/include/nu32.h"
#include "../libs/include/ltc6803.h"
#include "../libs/include/ad7685.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_2
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

#define LTC6803_COUNT 1
#define OVER_VOLTAGE 4.3
#define UNDER_VOLTAGE 3.0

/*
 * 
 */
int main(void)
{
    nu32_init(115200);

    struct ltc6803      ltc;
    struct ltc6803      *ltcp       = &ltc;

    struct ad7685       adc;
    struct ad7685       *adcp       = &adc;

    /* union BpsConfig cfg[LTC6803_COUNT];
    memset(&cfg, 0, sizeof(cfg));
    cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(OVER_VOLTAGE);
    cfg[0].vuv = convertUVLimit(UNDER_VOLTAGE); */
    int err;
    float voltages[1];
    float temp;
    err = ad7685_new(adcp, SPI_CHANNEL4, IOPORT_G, BIT_9, 1, THREE_WIRE, USE_BUSY_INDICATOR);
//     err = adcp->op.getVolts(adcp, voltages);
    temp = voltages[0];
    Nop();
    return 0;
}

