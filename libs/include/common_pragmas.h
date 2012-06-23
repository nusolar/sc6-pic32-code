#ifndef __NU_COMMON_PRAGMAS_H
#define __NU_COMMON_PRAGMAS_H

/* config pragmas needed mostly for PICKit3 */
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

#endif