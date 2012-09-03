#include "main.h"

int32_t
main(void)
{
    int32_t err = 0;

    EnableWDT();
    nu32_init(SYS_CLK_HZ);
    ClearWDT();

    struct can      commonCan;
    struct can      *commonCanp = &commonCan;
    struct serial   ser;
    struct serial   *serp = &ser;

    if ((err = can_new_easy(commonCanp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED)) < 0)
        ;   /* @TODO error handling */

    commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_CHN,
            CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_TX_RTR_DISABLED,
            CAN_HIGH_MEDIUM_PRIORITY, 0);

    commonCanp->op->addChannelRx(commonCanp, COMMON_CAN_RX_CHN,
            CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_RX_DATA_ONLY, 0);

    BYTE serDelims[] = {'\r', '\n'};
    if ((err = serial_new(serp, UART3, 115200, NO_UART_INTERRUPT,
            INT_PRIORITY_DISABLED, 0, UART_DATA_SIZE_8_BITS, 0,
            UART_ENABLE|UART_RX|UART_TX, serDelims, sizeof(serDelims))) < 0)
        ;

    while (1) {
        ClearWDT();

        uint64_t canRxBuf = 0;
        uint32_t id = 0;
        int32_t len = commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN,
            &id, &canRxBuf);
        if (len >= 0)
            serp->op->printf(serp, "t%03X%01d%X\r", id, len, canRxBuf);

        char serRxBuf[1000];
        if (serp->op->rx(serp, &serRxBuf, sizeof(serRxBuf)) >= 0) {
            uint16_t id = 0;
            uint32_t len = 0;
            uint64_t data = 0;
            if (sscanf(serRxBuf, "t%3X%1u%X", &id, &len, &data) > 0)
                commonCanp->op->tx(commonCanp, COMMON_CAN_TX_CHN, STANDARD_ID,
                        id, 0, 0, &data, len);
        }
    }

    return 0;
}