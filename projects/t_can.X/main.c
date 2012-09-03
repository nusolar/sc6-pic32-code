#include "nu32.h"
#include "can.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct can can; struct can *canp = &can;
    struct can can2; struct can *canp2 = &can2;
    uint32_t id;
    char rxBuf[8] = {0};

    nu32_init(80000000);

    can_new_easy(canp2, CAN1, 0, INT_PRIORITY_DISABLED);
    canp2->op->addChannelRx(canp2, CAN_CHANNEL1, CAN_TX_RX_MESSAGE_SIZE_BYTES,
            CAN_RX_DATA_ONLY, 0);

    can_new_easy(canp, CAN2, 0, INT_PRIORITY_DISABLED);
    canp->op->addChannelTx(canp, CAN_CHANNEL0, CAN_TX_RX_MESSAGE_SIZE_BYTES,
            CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);

    canp->op->tx(canp, CAN_CHANNEL0, STANDARD_ID, 1, 0, 0, "hello", 5);

    while (1) {
        canp->op->tx(canp, CAN_CHANNEL0, STANDARD_ID, 1, 0, 0, "hello", 5);
        if (canp2->op->rx(canp2, CAN_CHANNEL1, &id, rxBuf) >= 0)
            Nop();
    }

    while (1)
        ; /* do nothing */

    return 0;
}