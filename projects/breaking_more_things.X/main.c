#include <plib.h>

#include "../libs/include/nu32.h"
#include "../libs/include/spi.h"
#include "../libs/include/nokia5110.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

#define SYS_CLK_HZ  80000000

int main(void)
{
    nu32_init(115200);

    struct nokia5110 display;
    struct nokia5110 *dp = &display;
    nokia5110_new(dp, SPI_CHANNEL3,
                            IOPORT_E, BIT_2, IOPORT_E, BIT_1, IOPORT_E, BIT_0);


    struct spiPort adc;
    struct spiPort *adcp = &adc;
    SPI_CS_new(adcp, SPI_CHANNEL4, 100000,
        SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON,
                    AUTO_CS_PIN_DISABLE, IOPORT_G, BIT_9);


    while (1) {
        dp->op->clear(dp);

        static int i = 0;
        i++;
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "%d", i);

        delay_ms(1);
        adcp->op->driveCSHigh(adcp);
        delay_us(20);
        adcp->op->driveCSLow(adcp);

        unsigned int data = 0;

        adcp->op->rx(adcp, &data, 16);
        adcp->op->driveCSHigh(adcp);
        
        data = data & 0x0000FFFF;
        data = ((data>>8) & 0x00FF) | ((data<<8) & 0xFF00);

        float voltage = (5*((float) data))/65536;

        float offset = 0.4;
        float current = ((voltage - 2.5)/0.0125) - offset;

        dp->op->gotoXY(dp, 0, 1);
        dp->op->printf(dp, "%X", data);
        dp->op->gotoXY(dp, 0, 2);
        dp->op->printf(dp, "%d", data);
        dp->op->gotoXY(dp, 0, 3);
        dp->op->printf(dp, "%f", voltage);
        dp->op->gotoXY(dp, 0, 4);
        dp->op->printf(dp, "%f", current);

    }

    return (EXIT_SUCCESS);
}
