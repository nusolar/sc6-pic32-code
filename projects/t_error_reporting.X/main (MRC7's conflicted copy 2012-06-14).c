#include "can.h"
#include "error_reporting.h"
#include "led.h"
#include "nokia5110.h"
#include "nu32.h"
#include "serial.h"
#include "timer.h"

/* config pragmas needed mostly for PICKit3: */
/* ICE/ICD Comm Channel Select: ICE EMUC2/EMUD2 pins shared with PGC2/PGD2 */
#pragma config ICESEL   = ICS_PGx2
/* PLL Multiplier: 20x Multiplier */
#pragma config FPLLMUL  = MUL_20
/* PLL Input Divider: 2x Divider*/
#pragma config FPLLIDIV = DIV_2
/* System PLL Output Clock Divider: PLL Divide by 1 */
#pragma config FPLLODIV = DIV_1
/* Periphearal [Bus] Clock Divisor: Pb_Clk = Sys_Clk/1 */
#pragma config FPBDIV   = DIV_1
/* Primary Oscillator Configuration Mode: XT osc mode */
#pragma config POSCMOD  = XT
/* Oscillator Selection Bits: Primary Oscillator w/ PLL */
#pragma config FNOSC = PRIPLL

/* disable WDT (we'll control it in software) */
#pragma config FWDTEN   = OFF

/* 2.048s WDT timeout (1:2048 postscaler) */
#pragma config WDTPS    = PS2048

const uint32_t      SYS_CLK_HZ              = 80000000; /* 80 MHz */

int
main(void)
{
    struct can          can,        *canp   = &can;
    struct serial       ser,        *serp   = &ser;
    struct led          led0,       *led0p  = &led0;
    struct led          led1,       *led1p  = &led1;
    struct nokia5110    display,    *dp     = &display;

    /* setup clock, interrupts, NU32 LED pinouts, and switch input */
    nu32_init();

    led_new(led0p, NU32_LED0_PORT, NU32_LED0_PIN);
    led_new(led1p, NU32_LED1_PORT, NU32_LED1_PIN);

    char delims[2] = {'\r', '\n'};
    serial_new(serp, NU32_UART_MODULE, 9600, NO_UART_INTERRUPT,
            INT_PRIORITY_DISABLED, 0, UART_DATA_SIZE_8_BITS, 0,
            UART_ENABLE|UART_TX|UART_RX, delims, sizeof(delims));

    nokia5110_new(dp, SPI_CHANNEL3, IOPORT_E, BIT_2, IOPORT_E, BIT_1,
                IOPORT_E, BIT_0);

    can_new_easy(canp, CAN2, 0, INT_PRIORITY_DISABLED);
    canp->error_reporting_can_chn = CAN_CHANNEL0;
    canp->error_reporting_can_use_extended_id = STANDARD_ID;
    canp->error_reporting_can_std_id = ADDR_BMSTX(ERROR);
    canp->error_reporting_can_ext_id = 0;

    canp->op->addChannelTx(canp, CAN_CHANNEL0,
        CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_TX_RTR_DISABLED,
        CAN_HIGH_MEDIUM_PRIORITY, 0);

    delay(5);
    
    register_reporting_dev(&(led0p->erd), REP_DEBUG);
    REPORTF((REP_DEBUG, -1, "", "Testing %d", 5)); /* should light led0 */

    delay(5);
    
    err_clear(REP_DEBUG); /* should clear led0 */

    delay(5);

    REPORT((REP_WARNING, "Testing %d", 5)); /* should do nothing */
    unregister_reporting_dev(&(led0p->erd));
    register_reporting_dev(&(led0p->erd), REP_WARNING);
    register_reporting_dev(&(led1p->erd), REP_ERROR);
    REPORT((REP_WARNING, "Testing %d", 5)); /* should light led0 only */

    while (1)
        ;   /* do nothing */
    
    return 0;
}
