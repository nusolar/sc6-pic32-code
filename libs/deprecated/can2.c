#include "can.h"

#include "can.h"
#include "errorcodes.h"
#include "timer.h"
#include <string.h>

#define DEFAULT_BUS_SPEED_HZ    1E6
#if 0
typedef struct
{
    /* Number of Time quanta in Phase Segment 2.
     * This value can be between CAN_BIT_1TQ and CAN_BIT_8TQ */
	CAN_BIT_TQ 	phaseSeg2Tq;

    /* Number of Time quanta in Phase Segment 1.
     * This value can be between CAN_BIT_1TQ and CAN_BIT_8TQ */
	CAN_BIT_TQ 	phaseSeg1Tq;

    /* Number of Time quanta in Propagation Segment 2.
     * This value can be between CAN_BIT_1TQ and CAN_BIT_8TQ */
	CAN_BIT_TQ 	propagationSegTq;

    /* This determines if the Phase Segment 2  value is
     * specified by code or set automatically by the module.
     * TRUE - Phase Segment 2 can be set in code.
     * FALSE - Phase Segment 2 is set by module.
     */
	BOOL	 	phaseSeg2TimeSelect;

    /* This determines the number of times a bit is sampled by
     * the CAN module..
     * TRUE - bit is sampled 3 times.
     * FALSE - bit is sampled once.
     */
	BOOL 		sample3Time;

    /* This determines the Synchronization jump width Time quanta.
     * This value should be between CAN_BIT_1TQ and CAN_BIT_4TQ.
     */
	CAN_BIT_TQ 	syncJumpWidth;

}CAN_BIT_CONFIG;

static const CAN_BIT_CONFIG default_cfg = {
    /* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
    /* .phaseSeg2TimeSelect    = */ AUTO_SET,
    /* .phaseSeg2Tq            = */ CAN_BIT_5TQ,
    /* .propagationSegTq       = */ CAN_BIT_1TQ,
    /* .sample3Time            = */ THREE_TIMES,
    /* .syncJumpWidth          = */ CAN_BIT_1TQ
};
#endif

struct nu__Can__Attr {
    u32 bus_speed_hz;
    CAN_BIT_CONFIG timings;
    CAN_MODULE_EVENT interrupt_events;
    INT_PRIORITY int_priority;
    CAN_MODULE_FEATURES features;
    size_t buf_size;
};
static const struct nu__Can__Attr nu__Can__attr_defaults = {
    0
};

static ALWAYSINLINE MUST_CHECK s32
nu_switch_module_mode(const struct nu__Can *c, CAN_OP_MODE op_mode, u32 timeout_ms)
{
    u32 start = nu__Timer__us();
    CANSetOperatingMode(c->module, op_mode);
    while (nu__Timer__us() - start < timeout_ms*1000)
        if (CANGetOperatingMode(c->module) == op_mode)
            return 0;
    return -ETIMEOUT;
}

#define go_config_mode(c)   nu_switch_module_mode((c), CAN_CONFIGURATION, 1)
#define go_normal_mode(c)   nu_switch_module_mode((c), CAN_NORMAL_OPERATION, 1)

static ALWAYSINLINE MUST_CHECK s32
nu_change_features(const struct nu__Can *c, CAN_MODULE_FEATURES features, BOOL enabled)
{
    s32 err;
    if ((err = go_config_mode(c)) < 0) {
        go_normal_mode(c);
        return err;
    }
    CANEnableFeature(c->module, features, enabled);

    return 0;
}

#define enable_features(nu_can, features)  nu_change_features((nu_can), (features), TRUE)
#define disable_features(nu_can, features) nu_change_features((nu_can), (features), FALSE)

static ALWAYSINLINE MUST_CHECK s32
nu__Can__init(struct nu__Can *c, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features)
{
    s32 err;

    CANEnableModule(c->module, TRUE);

    if ((err = go_config_mode(c)) <0) {
        go_normal_mode(c);
        return err;
    }

    CANSetSpeed(c->module, timings, NU_HZ, bus_speed_hz);

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
nu__Can__tx(const struct nu__Can *c, CAN_CHANNEL chn, enum nu__Can__id_type id_type,
        u16 sid, u32 eid, u32 rtr, const void *data, size_t n)
{
    s32 err;

    CANTxMessageBuffer *message = CANGetTxMessageBuffer(c->module, chn);

    if (n > 8)
        return -EINVAL;

    if ((err = go_normal_mode(c)) < 0)
        return err;

    /* clear message data */
    memset(message, 0, sizeof(CANTxMessageBuffer));

    /* insert SID/EID information */
    message->msgEID.IDE = (NU_CAN_EXTENDED_ID == id_type);
    message->msgSID.SID = BITFIELD_CAST(sid, 11);  /* 11   bits */
    message->msgEID.DLC = BITFIELD_CAST(n, 4);   /* 4    bits */
    message->msgEID.RTR = BITFIELD_CAST(rtr, 1);   /* 1    bit; 1 = remote transmission request enabled */
    if (NU_CAN_EXTENDED_ID == id_type) /* EID is indicated by IDTypeExtended = 1 */
        message->msgEID.EID = BITFIELD_CAST(eid, 18);    /* 18 bits */

    if (n)
        memcpy(message->data, (const byte *)data, n);

    CANUpdateChannel(c->module, chn);
    CANFlushTxChannel(c->module, chn);

    return 0;
}

#define send_err_frame(nu_can, data, n)                                        \
    nu__Can__tx( (nu_can), (nu_can)->erd_chn, (nu_can)->erd_id_type, (nu_can)->erd_std_id,   \
            (nu_can)->erd_ext_id, 0, (data), (n))

COLD s32
nu__Can__report(struct nu__ErrorReportingDev *erd,
    const char *file, u32 line, const char *expr,
    enum nu_report_priority priority, s32 err, const char *err_name,
    const char *fmtd_msg)
{
    struct nu__Can *nu_can;
    union can_anyFrame frame = {
        .common.tx.errPriorityNum.errNum = err,
        .common.tx.errPriorityNum.priority = priority,
    };
    u32 ui;
    char txBuf[128];    /* should be multiple of 8 */
    STATIC_ASSERT(!(sizeof(txBuf)%8), SIZE_MISMATCH);

    nu_can = nu_erd_to_can(erd);

    send_err_frame(nu_can, NULL, 0);
    send_err_frame(nu_can, &frame.common.tx.errPriorityNum,
                    sizeof(frame.common.tx.errPriorityNum));

    snprintf(   txBuf, sizeof(txBuf),
                "err %d(%s):%s\r\n"
                "\tat %s:%d with call to\r\n"
                "\t%s\r\n",
                err, err_name, fmtd_msg, file, line, expr);

    for (ui = 0; ui < sizeof(txBuf); ui += 8) {
        nu__WDT__clear();
        send_err_frame(nu_can, txBuf+ui, 8);
    }

    return 0;
}

s32
nu__Can__new(struct nu__Can *c, CAN_MODULE mod, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features)
{
    c->erd.op = &nu__Can__erd_ops;
    c->module = mod;

    if (unlikely(!timings))
        timings = &default_cfg;
    if (unlikely(!bus_speed_hz))
        bus_speed_hz = DEFAULT_BUS_SPEED_HZ;

    return nu__Can__init(c, bus_speed_hz, timings, interrupt_events, int_priority,
                    features);
}

s32
nu__Can__add_channel_tx(const struct nu__Can *c, CAN_CHANNEL channel,
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
nu__Can__add_channel_rx(const struct nu__Can *c, CAN_CHANNEL chn, u32 channel_msg_size,
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
nu__Can__rx(const struct nu__Can *c, CAN_CHANNEL chn, u32 *id, void *dst)
{
    CANRxMessageBuffer *buf;
    unsigned long len;

    buf = (CANRxMessageBuffer *)CANGetRxMessage(c->module, chn);
    if (NULL == buf)
        return -ENODATA;

    if (buf->msgEID.IDE == NU_CAN_STANDARD_ID) {
        *id = (int)(buf->msgSID.SID);
    } else {    /* NU_CAN_EXTENDED_ID */
        /* msgEID.EID is 18 bits; msgSID.SID is 11 bits */
        *id = (unsigned int)(buf->msgEID.EID) | (unsigned int)(buf->msgSID.SID << 18);
    }

    len = (unsigned long) buf->msgEID.DLC;

    memcpy(dst, buf->data, len);
    CANUpdateChannel(c->module, chn);

    return (long) len;
}

MUST_CHECK s32
nu__Can__add_filter(const struct nu__Can *c, CAN_CHANNEL chn, CAN_FILTER filter,
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

const struct nu__ErrorReportingDev__Vtbl nu__Can__erd_ops = {
    .report         = &nu__Can__report,
    .reset_err_state= NULL,
};
