#include "nu32.h"
#include "can.h"
#include "error_reporting.h"
#include "serial.h"

#include "common_pragmas.h"

static const uint32_t       SYS_CLK_HZ           = 80000000;

static const CAN_MODULE     COMMON_CAN_MOD       = CAN1;
static const CAN_CHANNEL    COMMON_CAN_TX_CHN    = CAN_CHANNEL0;
static const CAN_CHANNEL    COMMON_CAN_RX_CHN    = CAN_CHANNEL1;
static const uint32_t       SERIAL_BAUD          = 9600;

int32_t
main(void)
{
    struct can      commonCan,  *commonCanp = &commonCan;

    EnableWDT();

    nu32_init(SYS_CLK_HZ);

    nu32_init_serial(SERIAL_BAUD);

    can_new_easy(commonCanp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED);

    commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_CHN,
            32, CAN_TX_RTR_DISABLED,
            CAN_HIGH_MEDIUM_PRIORITY, 0);

    commonCanp->op->addChannelRx(commonCanp, COMMON_CAN_RX_CHN,
            32, CAN_RX_FULL_RECEIVE, 0);

    while (1) {
        char canRxBuf[8];
        int32_t len;
        uint32_t id;
        
        ClearWDT();

        if ((len = commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN,
                &id, canRxBuf)) >= 0) {
            uint32_t ui;
            nu32_serp->op->printf(nu32_serp,
                                    "\r\n"
                                    "t,id=0x%0X=%d,len=%d,data=0x", id, id, len);
            for (ui = 0; ui < len; ++ui)
                nu32_serp->op->printf(nu32_serp, "%02X", canRxBuf[ui]);
            nu32_serp->op->printf(nu32_serp, "\r\n");
        }

//        char serRxBuf[1000];
//        if (serp->op->rx(serp, &serRxBuf, sizeof(serRxBuf)) >= 0) {
//            uint16_t id = 0;
//            uint32_t len = 0;
//            uint64_t data = 0;
//            if (sscanf(serRxBuf, "t%3X%1u%X", &id, &len, &data) > 0)
//                commonCanp->op->tx(commonCanp, COMMON_CAN_TX_CHN, STANDARD_ID,
//                        id, 0, 0, &data, len);
//        }
    }

    return 0;
}