#include "nokia5110.h"
#include "nu32.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct nokia5110 display; struct nokia5110 *dp = &display;
    struct nokia5110 display2; struct nokia5110 *dp2 = &display2;

    nu32_init(80000000);

    nokia5110_new(dp, SPI_CHANNEL2,
            IOPORT_G, BIT_9,
            IOPORT_A, BIT_9,
            IOPORT_E, BIT_9);

    nokia5110_new(dp2, SPI_CHANNEL2,
            IOPORT_E, BIT_8,
            IOPORT_A, BIT_10,
            IOPORT_E, BIT_9);

    delay(1);

    dp->op->clear(dp);
    dp2->op->clear(dp2);
    dp->op->gotoXY(dp, 0, 0);
    dp2->op->gotoXY(dp2, 0, 0);

    delay(1);

    dp->op->printf(dp, "hello johnny! %d", 10);
    dp2->op->printf(dp2, "fun times...");

    delay(1);

    while (1)
        ; /* do nothing */

    return 0;
}
