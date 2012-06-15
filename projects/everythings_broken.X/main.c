#include <plib.h>

#include "../libs/include/nu32.h"
#include "../libs/include/ltc6803.h"
#include "../libs/include/spi.h"
#include "../libs/include/nokia5110.h"
#include "../libs/include/can.h"
#include "../libs/include/onewire.h"
#include "../libs/include/timer.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

#define SYS_CLK_HZ  80000000

int main(void)
{
    nu32_init(115200);
    led0_off();
    led1_off();

    /*
    struct oneWire ow;
    struct oneWire *owp = &ow;
    oneWire_new(owp, IOPORT_A, BIT_4);

    const char *str = "abcde";

    while (1)
        owp->op->tx(owp, str, sizeof(str));
     */
    
    struct nokia5110 display;
    struct nokia5110 *dp = &display;
    nokia5110_new(dp, SPI_CHANNEL3, IOPORT_E, BIT_2, IOPORT_E, BIT_1, IOPORT_E, BIT_0);

    led0_on();
    led1_off();

    unsigned int count = 0;
    while(1) {
        count++;
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "%d", count);
        led0_toggle();
        led1_toggle();
        delay_ms(20.5);
    }

    union BpsConfig cfg[2];
    memset(&cfg, 0, sizeof(cfg));
    cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
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
    if (!bpsp->op->cfgsMatch(bpsp, cfg)) {
            dp->op->gotoXY(dp, 0, 2);
            dp->op->printf(dp, "HOORAY!");
    }

    
    while (1) {
        dp->op->clear(dp);

        static int i = 0;
        i++;
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "%d", i);

        bpsp->op->writeCfgs(bpsp, cfg);

        int result = bpsp->op->cfgsMatch(bpsp, cfg);
        dp->op->gotoXY(dp, 0, 2);
        dp->op->printf(dp, "Result=%d", result);
        dp->op->gotoXY(dp, 0, 3);
        if (result < 0) {
            dp->op->printf(dp, "mcfgs=%X", bpsp->flags_mismatchCfgs);
            dp->op->gotoXY(dp, 0, 5);
            dp->op->printf(dp, "mpecs=%X", bpsp->flags_mismatchPecs);
        } else {
            dp->op->printf(dp, "HOORAY!");
        }

        bpsp->op->measureVolts(bpsp);
        float voltages[24];
        bpsp->op->readVolts(bpsp, voltages);

        dp->op->gotoXY(dp, 0, 4);
        dp->op->printf(dp, "V=%f", voltages[0]);
        dp->op->gotoXY(dp, 0, 5);
        dp->op->printf(dp, "V=%f", voltages[23]);
    }
     

    /*
    struct CANPort CANtest;
    struct CANPort *CANtestp = &CANtest;
    CAN_new(CANtestp, CAN1, 1E6, CAN_BIT_3TQ, CAN_BIT_5TQ, CAN_BIT_1TQ,
            AUTO_SET, THREE_TIMES, CAN_BIT_1TQ, 0, 0, 0);

    CANtestp->op->addChannelTx(CANtestp, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED,
                                CAN_LOW_MEDIUM_PRIORITY, 0);

    CANtestp->op->addChannelRx(CANtestp, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE, 0);

    UINT64 testData = 0;
    memset(&testData, 0xAC, sizeof(testData));

    while (1) {
        static int i = 0;
        i++;

        dp->op->clear(dp);
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "fun time! %d", i);

        testData++;
        CANtestp->op->tx(CANtestp, CAN_CHANNEL0, STANDARD_ID, 0x400, 0, 0,
           &testData, 5);

        unsigned int rxId = 0x404;
        UINT64 rxData = 0x1337;
        int rxResult = 0;
        static int lastFail = 0;
        if ((rxResult = CANtestp->op->rx(CANtestp, CAN_CHANNEL1, &rxId, &rxData)) > 0) {
            dp->op->gotoXY(dp, 0, 2);
            dp->op->printf(dp, "id %X", rxId);
            dp->op->gotoXY(dp, 0, 3);
            dp->op->printf(dp, "data %X", rxData);
        } else {
            lastFail = rxResult;
        }
        dp->op->gotoXY(dp, 0, 4);
        dp->op->printf(dp, "result %d", rxResult);
        
        dp->op->gotoXY(dp, 0, 5);
        dp->op->printf(dp, "lstfail %d", lastFail);
    }
    */

    return (EXIT_SUCCESS);
}
