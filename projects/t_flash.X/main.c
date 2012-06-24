/*
 * On first run, only LED1 should light. On subsequent runs, both LEDs should
 * light.
 */

#include <string.h>
#include "flash.h"
#include "nu32.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    const char *test = "hello";
    char tmp[6] = {0};
    int32_t ret;

    nu32_init(80000000);

    PORTSetPinsDigitalOut(NU32_LED0_PORT, NU32_LED0_PIN);
    PORTSetPinsDigitalOut(NU32_LED1_PORT, NU32_LED1_PIN);

    /* turn leds off */
    PORTSetBits(NU32_LED0_PORT, NU32_LED0_PIN);
    PORTSetBits(NU32_LED1_PORT, NU32_LED1_PIN);

    delay(1);

    readFlash(tmp, sizeof(tmp));
    if (!(ret = strcmp(test, tmp))) {
        /* turn on led0 (L2/LED3 on new NU32s) */
        PORTClearBits(NU32_LED0_PORT, NU32_LED0_PIN);
    }

    writeFlash(test, strlen(test));
    readFlash(tmp, sizeof(tmp));

    if (!(ret = strcmp(test, tmp))) {
        /* turn on led1 (L1/LED2 on new NU32s) */
        PORTClearBits(NU32_LED1_PORT, NU32_LED1_PIN);
    }
    
    while (1)
        ;   /* do nothing */

    return 0;
}
