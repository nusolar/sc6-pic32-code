#include "nu/peripheral/can/add_channel_tx.h"
#include "nu/peripheral/can.h"

struct nu__Can__AddChannelTxAttr {
    CAN_CHANNEL chn;
    u32 channel_msg_size;
    CAN_TX_RTR rtr_enabled;
    CAN_TXCHANNEL_PRIORITY priority;
    CAN_CHANNEL_EVENT interrupt_events;
};

void
nu__Can__add_channel_tx(const struct nu__Can *c,
                      const struct nu__Can__AddChannelTxAttr *a)
{
    CANSetOperatingMode(c->module, CAN_CONFIGURATION);
    while (CAN_CONFIGURATION != CANGetOperatingMode(c->module))
        Nop();

    CANConfigureChannelForTx(c->module, a->chn, a->channel_msg_size,
        a->rtr_enabled, a->priority);
    CANEnableChannelEvent(c->module, a->chn, a->interrupt_events, TRUE);

    CANSetOperatingMode(c->module, CAN_NORMAL_OPERATION);
    while (CAN_NORMAL_OPERATION != CANGetOperatingMode(c->module))
        Nop();
}
