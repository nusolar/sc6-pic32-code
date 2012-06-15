#include <plib.h>
#include <stdio.h>
#include <string.h>
#include "../libs/include/Nokia5110.h"
#include "../libs/include/NUCAN.h"
#include "../libs/include/NU32v2.h"
#include "../libs/include/SolarCan.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

int
main (void)
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);

    // configure for multi-vectored mode
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    initSerialNU32v2();
    initLEDs();
    LED0_OFF();
    LED1_OFF();

    // init and clear the LCD
    LcdInitialize();
    LcdClear();

    NUCANInitializeModule(CAN1, FALSE);

    while (1) {
        union ANY_FRAME canFrame    = {0};
        unsigned int id, len;
        if (!NUCANRx(CAN1, &id, canFrame.dataByte, &len)) {
            LED0_ON();
            char buff[50];
            snprintf(buff, strlen(buff), "t%04X%d", id, len);
            unsigned int i;
            for (i = 0; i < len; ++i)
                snprintf(buff, strlen(buff), "%s%02X", buff, canFrame.dataByte[i]);
            snprintf(buff, strlen(buff), "%s\r\n\r\n", buff);
            gotoXY(0,0);
            LcdClear();
            LcdString(buff);
            LED0_OFF();
        }
    }

    return 0;
}
