#include "nu32.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    uint32_t tmp = 0;

    nu32_init(80000000);
    
    PORTSetPinsDigitalOut(IOPORT_D, BIT_2);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_3);

    PORTClearBits(IOPORT_D, BIT_2);
    PORTClearBits(IOPORT_D, BIT_3);

    PORTSetBits(IOPORT_D, BIT_2);
    PORTSetBits(IOPORT_D, BIT_3);

    PORTClearBits(IOPORT_D, BIT_2);
    PORTClearBits(IOPORT_D, BIT_3);

    PORTSetBits(IOPORT_D, BIT_2);
    PORTSetBits(IOPORT_D, BIT_3);

    PORTSetPinsDigitalIn(IOPORT_B, ~0UL);
    while(1) {
        tmp = PORTRead(IOPORT_B);
        Nop();
    }

    return 0;
}