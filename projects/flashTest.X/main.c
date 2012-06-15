#include <plib.h>
#include <stdio.h>
#include <stdlib.h>

/* config pragmas needed mostly for PICKit3 */
#pragma config ICESEL   = ICS_PGx2
#pragma config FPLLMUL  = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV   = DIV_1
#pragma config POSCMOD  = XT, FNOSC = PRIPLL

/* disable WDT (we'll control it in software) */
#pragma config FWDTEN   = OFF

/* 2.048s WDT timeout (1:2048 postscaler) */
#pragma config WDTPS    = PS2048

int main(int argc, char** argv)
{
    nu32_init(115200);

    NVMErasePage((void *)0xbd008000);
    

    return (EXIT_SUCCESS);
}

