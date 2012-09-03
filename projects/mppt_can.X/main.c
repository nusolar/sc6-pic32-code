#include "nu32.h"
#include "can.h"
#include "can_all.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct can can; struct can *canp = &can;
    uint32_t id;
    union can_mppt mppt;

    nu32_init(80000000);
    nu32_init_serial(9600);

    can_new(canp, CAN1, 125E3, CAN_BIT_2TQ, CAN_BIT_4TQ, CAN_BIT_1TQ,
            AUTO_SET, THREE_TIMES, CAN_BIT_1TQ, 0, INT_PRIORITY_DISABLED, 0);

//    can_new_easy(canp, CAN1, 0, INT_PRIORITY_DISABLED);

    canp->op->addChannelTx(canp, CAN_CHANNEL0, 32,
            CAN_TX_RTR_ENABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
    canp->op->addChannelRx(canp, CAN_CHANNEL1, 32,
            CAN_RX_FULL_RECEIVE, 0);

    while (1) {
        canp->op->tx(canp, CAN_CHANNEL0, STANDARD_ID, ADDR_MPPT3_RX_BASE, 0, 1, NULL, 0);
        if (canp->op->rx(canp, CAN_CHANNEL1, &id, &(mppt.tx.frame)) > 0 && (id == ADDR_MPPT3_TX_FRAME))
            Nop();
        if (canp->op->rx(canp, CAN_CHANNEL1, &id, &(mppt.tx.frame)) > 0 && (id == ADDR_MPPT3_TX_FRAME))
            Nop();
        delay_ms(1);
    }

    while (1)
        ; /* do nothing */

    return 0;
}
