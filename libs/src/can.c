#include <string.h>
#include "can.h"
#include "errorcodes.h"
#include "timer.h"

#define DEFAULT_BUS_SPEED_HZ    1E6
static const CAN_BIT_CONFIG default_cfg = {
    .phaseSeg1Tq            = CAN_BIT_3TQ,
    .phaseSeg2TimeSelect    = AUTO_SET,
    .phaseSeg2Tq            = CAN_BIT_5TQ,
    .propagationSegTq       = CAN_BIT_1TQ,
    .sample3Time            = THREE_TIMES,
    .syncJumpWidth          = CAN_BIT_1TQ
};

static ALWAYSINLINE MUST_CHECK s32
switch_module_mode(const struct can *c, CAN_OP_MODE op_mode, u32 timeout_ms)
{
    u32 start = timer_us();
    CANSetOperatingMode(c->module, op_mode);
    while (timer_us() - start < timeout_ms*1000)
        if (CANGetOperatingMode(c->module) == op_mode)
            return 0;
    return -ETIMEOUT;
}

#define go_config_mode(c)   switch_module_mode((c), CAN_CONFIGURATION, 1)
#define go_normal_mode(c)   switch_module_mode((c), CAN_NORMAL_OPERATION, 1)

static ALWAYSINLINE MUST_CHECK s32
change_features(const struct can *c, CAN_MODULE_FEATURES features, BOOL enabled)
{
    s32 err;
    if ((err = go_config_mode(c)) < 0) {
        go_normal_mode(c);
        return err;
    }
    CANEnableFeature(c->module, features, enabled);

    return 0;
}

#define enable_features(can, features)  change_features((can), (features), TRUE)
#define disable_features(can, features) change_features((can), (features), FALSE)

static ALWAYSINLINE MUST_CHECK s32
can_init(const struct can *c, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features)
{
    s32 err;

    CANEnableModule(c->module, TRUE);

    if ((err = go_config_mode(c)) <0) {
        go_normal_mode(c);
        return err;
    }

    CANSetSpeed(c->module, timings, HZ, bus_speed_hz);

    CANAssignMemoryBuffer(c->module, c->buf, sizeof(c->buf));

    if (interrupt_events) {
        INT_VECTOR int_vec;
        INT_SOURCE int_src;

        CANEnableModuleEvent(CAN1, interrupt_events, TRUE);

        switch (c->module) {
        case CAN1:
            int_vec = INT_CAN_1_VECTOR;
            int_src = INT_CAN1;
            break;
        case CAN2:
            int_vec = INT_CAN_2_VECTOR;
            int_src = INT_CAN2;
            break;
        case CAN_NUMBER_OF_MODULES:
        default:
            break;
        }

        INTSetVectorPriority(int_vec, int_priority);
        INTSetVectorSubPriority(int_vec, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(int_src, INT_ENABLED);
    }

    enable_features(c, features);

    if ((err = go_normal_mode(c)) < 0)
        return err;

    return 0;
}

MUST_CHECK s32
can_tx(const struct can *c, CAN_CHANNEL chn, enum id_type id_type,
        u16 sid, u32 eid, u32 rtr, const void *data, size_t n)
{
    s32 err;

    CANTxMessageBuffer *message = CANGetTxMessageBuffer(c->module, chn);

    if (n > 8)
        return -EINVAL;

    if ((err = go_normal_mode(self)) < 0)
        return err;

    /* clear message data */
    memset(message, 0, sizeof(CANTxMessageBuffer));

    /* insert SID/EID information */
    message->msgEID.IDE = (EXTENDED_ID == id_type);
    message->msgSID.SID = BITFIELD_CAST(sid, 11);  /* 11   bits */
    message->msgEID.DLC = BITFIELD_CAST(len, 4);   /* 4    bits */
    message->msgEID.RTR = BITFIELD_CAST(rtr, 1);   /* 1    bit; 1 = remote transmission request enabled */
    if (EXTENDED_ID == id_type) /* EID is indicated by IDTypeExtended = 1 */
        message->msgEID.EID = BITFIELD_CAST(eid, 18);    /* 18 bits */

    if (n)
        memcpy(message->data, (const byte *)data, n);

    CANUpdateChannel(c->module, chn);
    CANFlushTxChannel(c->module, chn);

    return 0;
}

#define send_err_frame(can, data, n)                                        \
    can_tx( (can), (can)->erd_chn, (can)->erd_id_type, (can)->erd_std_id,   \
            (can)->erd_ext_id, 0, (data), (n))

COLD s32
can_report(struct error_reporting_dev *erd,
    const char *file, u32 line, const char *expr,
    enum report_priority priority, s32 err, const char *err_name,
    const char *fmtd_msg)
{
    struct can *can;
    union can_anyFrame frame = {
        .common.tx.errPriorityNum.errNum = err,
        .common.tx.errPriorityNum.priority = priority,
    };
    u32 ui;
    char txBuf[128];    /* should be multiple of 8 */
    STATIC_ASSERT(!(sizeof(txBuf)%8), SIZE_MISMATCH);

    can = erd_to_can(erd);

    send_err_frame(can, NULL, 0);
    send_err_frame(can, &frame.common.tx.errPriorityNum,
                    sizeof(frame.common.tx.errPriorityNum));

    snprintf(   txBuf, sizeof(txBuf),
                "err %d(%s):%s\r\n"
                "\tat %s:%d with call to\r\n"
                "\t%s\r\n",
                err, err_name, fmtd_msg, file, line, expr);

    for (ui = 0; ui < sizeof(txBuf); ui += 8) {
        clear_wdt();
        send_err_frame(can, txBuf+ui, 8);
    }

    return 0;
}

s32
can_new(struct can *c, CAN_MODULE mod, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features)
{
    c->erd.op = &can_erd_ops;
    c->module = mod;

    if (unlikely(!timings))
        timings = &default_cfg;
    if (unlikely(!bus_speed_hz))
        bus_speed_hz = DEFAULT_BUS_SPEED_HZ;

    return can_init(c, bus_speed_hz, timings, interrupt_events, int_priority,
                    features);
}

s32
can_add_channel_tx(const struct can *c, CAN_CHANNEL channel,
            u32 channel_msg_size, CAN_TX_RTR rtr_enabled,
            CAN_TXCHANNEL_PRIORITY tx_priority, CAN_CHANNEL_EVENT interrupt_events)
{
    s32 err;

    if ((err = go_config_mode(c)) < 0) {
        go_normal_mode(c);
        return err;
    }

    CANConfigureChannelForTx(c->module, channel, channel_msg_size, rtr_enabled, tx_priority);
    CANEnableChannelEvent(c->module, channel, interrupt_events, TRUE);

    if ((err = go_normal_mode(c)) < 0)
        return err;

    return 0;
}

s32
can_add_channel_rx(const struct can *c, CAN_CHANNEL chn, u32 channel_msg_size,
                    CAN_RX_DATA_MODE data_only, CAN_CHANNEL_EVENT interrupt_events)
{
    s32 err;

    if ((err = go_config_mode(c)) < 0) {
        go_normal_mode(c);
        return err;
    }

    CANConfigureChannelForRx(c->module, chn, channel_msg_size, data_only);
    CANConfigureFilter(c->module, CAN_FILTER0, 0, CAN_SID);
    CANConfigureFilterMask(c->module, CAN_FILTER_MASK0, 0, CAN_SID, CAN_FILTER_MASK_ANY_TYPE);
    CANLinkFilterToChannel(c->module, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
    CANEnableFilter(c->module, CAN_FILTER0, TRUE);
    CANEnableChannelEvent(c->module, chn, interrupt_events, TRUE);
    
    if ((err = go_normal_mode(c)) < 0)
        return err;

    return 0;
}

MUST_CHECK long
can_rx(const struct can *c, CAN_CHANNEL chn, u32 *id, void *dst)
{
    CANRxMessageBuffer *buf;
    unsigned long len;

    buf = (CANRxMessageBuffer *)CANGetRxMessage(c->module, chn);
    if (NULL == buf)
        return -ENODATA;

    if (buf->msgEID.IDE == STANDARD_ID) {
        *id = (int)(buf->msgSID.SID);
    } else {    /* EXTENDED_ID */
        /* msgEID.EID is 18 bits; msgSID.SID is 11 bits */
        *id = (unsigned int)(buf->msgEID.EID) | (unsigned int)(buf->msgSID.SID << 18);
    }

    len = (unsigned long) buf->msgEID.DLC;

    memcpy(dst, buf->data, len);
    CANUpdateChannel(c->module, chn);

    return (long) len;
}

MUST_CHECK s32
can_add_filter(const struct can *c, CAN_CHANNEL chn, CAN_FILTER filter,
                CAN_ID_TYPE filter_type, u32 id, CAN_FILTER_MASK mask,
                CAN_FILTER_MASK_TYPE mide, u32 mask_bits)
{
    s32 err;

    if ((err = go_config_mode(c)) < 0) {
        go_normal_mode(c);
        return err;
    }

    CANConfigureFilter(c->module, filter, id, filter_type);
    CANConfigureFilterMask(c->module, mask, mask_bits, filter_type, mide);
    CANLinkFilterToChannel(c->module, filter, mask, chn);
    CANEnableFilter(c->module, filter, TRUE);

    if ((err = go_normal_mode(c)) < 0)
        return err;

    return 0;
}

const struct vtbl_error_reporting_dev can_erd_ops = {
    .report         = &can_report,
    .reset_err_state= NULL,
};
