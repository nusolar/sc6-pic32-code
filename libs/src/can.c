#include "../include/can.h"

static int32_t CAN_init(struct can *self, unsigned int canBusSpeed,
                CAN_BIT_TQ phaseSeg2Tq, CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                enum PhaseSeg2TimeSelect selectAutoSet, enum SampleTimes sample3Times,
                CAN_BIT_TQ syncJumpWidth, CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority, CAN_MODULE_FEATURES moduleFeatures);

static ALWAYSINLINE int32_t
enableFunctions(const struct can *self, CAN_MODULE_FEATURES features);

static ALWAYSINLINE int32_t
disableFunctions(const struct can *self, CAN_MODULE_FEATURES features);

static int32_t
switchModuleOpMode(const struct can *self, CAN_OP_MODE opMode, double waitTimeMs);

static int32_t
putCanInConfigMode (const struct can *self);

static int32_t
putCanInNormalMode (const struct can *self);

static int32_t
CANTx (const struct can *self, CAN_CHANNEL channel, enum IDType IDTypeExtended,
        uint16_t sid, uint32_t eid, uint32_t rtr, const void *data, size_t len);

static long
CANRx (const struct can *self, CAN_CHANNEL channel, unsigned int *id,
        void *dest);

static int
addChannelTx(const struct can *self, CAN_CHANNEL channel,
            unsigned int channelMsgSize, CAN_TX_RTR rtrEnabled,
            CAN_TXCHANNEL_PRIORITY txPriority, CAN_CHANNEL_EVENT interruptEvents);

static int
addChannelRx(const struct can *self, CAN_CHANNEL channel,
            unsigned int channelMsgSize, CAN_RX_DATA_MODE dataOnly,
            CAN_CHANNEL_EVENT interruptEvents);

static int32_t
addFilter(const struct can *self, CAN_CHANNEL chn,
            CAN_FILTER filter, CAN_ID_TYPE filterType,
            uint32_t id, CAN_FILTER_MASK mask,
            CAN_FILTER_MASK_TYPE mide, uint32_t maskBits);

static const struct vtblCANPort CANPortOps = {
    .tx             = &CANTx,
    .rx             = &CANRx,
    .addChannelTx   = &addChannelTx,
    .addChannelRx   = &addChannelRx,
    .addFilter      = &addFilter,
};

static int32_t
can_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg);

static const struct vtblError_reporting_dev can_erd_ops = {
    .report         = &can_report,
    .resetErrState  = NULL,
};

int32_t
can_new(struct can *self, CAN_MODULE module,
        uint32_t canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
        CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq, enum PhaseSeg2TimeSelect selectAutoSet,
        enum SampleTimes sample3Times, CAN_BIT_TQ syncJumpWidth,
        CAN_MODULE_EVENT interruptEvents, INT_PRIORITY intPriority,
        CAN_MODULE_FEATURES moduleFeatures)
{
    if (self == NULL)
        return -ENULPTR;

    self->op = &CANPortOps;
    self->erd.op = &can_erd_ops;
    self->module = module;

    return CAN_init(self, canBusSpeedHz, phaseSeg2Tq, phaseSeg1Tq, propSegTq, selectAutoSet,
            sample3Times, syncJumpWidth, interruptEvents, intPriority, moduleFeatures);
}

int32_t
can_new_easy (struct can *self, CAN_MODULE module,
                CAN_MODULE_EVENT interruptEvents, INT_PRIORITY intPriority)
{
    if (self == NULL)
        return -ENULPTR;

    self->op = &CANPortOps;
    self->erd.op = &can_erd_ops;
    self->module = module;

    return CAN_init(self, 1E6, CAN_BIT_3TQ, CAN_BIT_5TQ, CAN_BIT_1TQ, AUTO_SET,
            THREE_TIMES, CAN_BIT_1TQ, interruptEvents, intPriority, 0);
}

static int32_t CAN_init(struct can *self, unsigned int canBusSpeedHz,
                CAN_BIT_TQ phaseSeg2Tq, CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                enum PhaseSeg2TimeSelect selectAutoSet, enum SampleTimes sample3Times,
                CAN_BIT_TQ syncJumpWidth, CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority, CAN_MODULE_FEATURES moduleFeatures)
{
    int32_t err;
    CAN_BIT_CONFIG bitCfg = {
        .phaseSeg2Tq            = phaseSeg2Tq,
        .phaseSeg1Tq            = phaseSeg1Tq,
        .propagationSegTq       = propSegTq,
        .phaseSeg2TimeSelect    = selectAutoSet,
        .sample3Time            = sample3Times,
        .syncJumpWidth          = syncJumpWidth,
    };

    if (self == NULL)
        return -ENULPTR;

    CANEnableModule(self->module, TRUE);

    if ((err = putCanInConfigMode(self)) <0) {
        putCanInNormalMode(self);
        return err;
    }

    CANSetSpeed(self->module, &bitCfg, sys_clk_hz, canBusSpeedHz);

    CANAssignMemoryBuffer(self->module, self->memoryBuffer, sizeof(self->memoryBuffer));

    if (interruptEvents) {
        INT_VECTOR intVect;
        INT_SOURCE intSrc;
        
        CANEnableModuleEvent(CAN1, interruptEvents, TRUE);
        
        switch (self->module) {
        case CAN1:
            intVect = INT_CAN_1_VECTOR;
            intSrc = INT_CAN1;
            break;
        case CAN2:
            intVect = INT_CAN_2_VECTOR;
            intSrc = INT_CAN2;
            break;
        case CAN_NUMBER_OF_MODULES:
        default:
            break;
        }

        INTSetVectorPriority(intVect, intPriority);
        INTSetVectorSubPriority(intVect, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(intSrc, INT_ENABLED);
    }

    enableFunctions(self, moduleFeatures);

    if ((err = putCanInNormalMode(self)) < 0)
        return err;

    return 0;
}

static int32_t
sendErrFrame(const struct can *self, void *data, size_t len)
{
    if (!self || (!data && len > 0))
        return -ENULPTR;

    return self->op->tx(self, self->error_reporting_can_chn,
        self->error_reporting_can_use_extended_id,
        self->error_reporting_can_std_id,
        self->error_reporting_can_ext_id, 0, data, len);
}

static ALWAYSINLINE void
ringSend(struct can *self, const char *msg, uint32_t *iter, char *txBuf)
{
    for ( ; *msg; *iter = (*iter+1)%8, ++msg) {
        txBuf[*iter] = *msg;
        if (*iter == 7)
            sendErrFrame(self, txBuf, 8);
    }
}

static int32_t
can_report(struct error_reporting_dev *self,
    const char *file, uint32_t line, const char *expr,
    enum report_priority priority, int32_t errNum, const char *errName,
    const char *fmtdMsg)
{
    struct can *canSelf;
    union can_anyFrame frame = {
        .common.tx.errPriorityNum.errNum = errNum,
        .common.tx.errPriorityNum.priority = priority,
    };
    uint32_t ui;
    char txBuf[128];    /* should be multiple of 8 */
    STATIC_ASSERT(!(sizeof(txBuf)%8), SIZE_MISMATCH);

    if (!self || !file || !expr || !errName || !fmtdMsg)
        return -ENULPTR;

    canSelf = error_reporting_dev_to_can(self);

    sendErrFrame(canSelf, NULL, 0);
    
    sendErrFrame(canSelf, &frame.common.tx.errPriorityNum,
                    sizeof(frame.common.tx.errPriorityNum));

    snprintf(   txBuf, sizeof(txBuf),
                "err %d(%s):%s\r\n"
                "\tat %s:%d with call to\r\n"
                "\t%s\r\n",
                errNum, errName, expr, file, line, expr);

    for (ui = 0; ui < sizeof(txBuf); ui += 8) {
        clear_wdt();
        sendErrFrame(canSelf, txBuf+ui, 8);
    }

    return 0;
}

static int
changeFunctions (const struct can *self, CAN_MODULE_FEATURES features, BOOL enabled)
{
    int error;

    if (self == NULL)
        return -ENULPTR;

    if ((error = putCanInConfigMode(self))) {
        putCanInNormalMode(self);
        return error;
    }

    CANEnableFeature(self->module, features, enabled);

    return 0;
}

static ALWAYSINLINE int32_t
enableFunctions(const struct can *self, CAN_MODULE_FEATURES features)
{
    return changeFunctions(self, features, TRUE);
}

static ALWAYSINLINE int32_t
disableFunctions(const struct can *self, CAN_MODULE_FEATURES features)
{
    return changeFunctions(self, features, FALSE);
}

static int32_t
switchModuleOpMode(const struct can *self, CAN_OP_MODE opMode, double waitTimeMs)
{
    double tStart;

    if (self == NULL)
        return -ENULPTR;

    if (CANGetOperatingMode(self->module) == opMode)
        return 0;

    CANSetOperatingMode(self->module, opMode);
    tStart = readTimer();
    while (CANGetOperatingMode(self->module) != opMode)
        ;   /* do nothing */

    return 0;
}

static int32_t
putCanInConfigMode (const struct can *self)
{
    int32_t err;
    if (self == NULL)
        return -ENULPTR;

    if ((err = switchModuleOpMode(self, CAN_CONFIGURATION, 1)) < 0)
        return err;

    return 0;
}

static int32_t
putCanInNormalMode (const struct can *self)
{
    int32_t err;
    if (self == NULL)
        return -ENULPTR;

    if ((err = switchModuleOpMode(self, CAN_NORMAL_OPERATION, 1)) < 0)
        return err;

    return 0;
}

static int
addChannelTx (const struct can *self, CAN_CHANNEL channel,
            unsigned int channelMsgSize, CAN_TX_RTR rtrEnabled,
            CAN_TXCHANNEL_PRIORITY txPriority, CAN_CHANNEL_EVENT interruptEvents)
{
    int error;

    if (self == NULL)
        return -ENULPTR;

    if ((error = putCanInConfigMode(self))) {
        putCanInNormalMode(self);
        return error;
    }

    CANConfigureChannelForTx(self->module, channel, channelMsgSize, rtrEnabled, txPriority);
    if (interruptEvents)
        CANEnableChannelEvent(self->module, channel, interruptEvents, TRUE);

    if ((error = putCanInNormalMode(self)))
        return error;

    return 0;
}

static int
addChannelRx (const struct can *self, CAN_CHANNEL channel,
            unsigned int channelMsgSize, CAN_RX_DATA_MODE dataOnly, CAN_CHANNEL_EVENT interruptEvents)
{
    int error;

    if (self == NULL)
        return -ENULPTR;

    if ((error = putCanInConfigMode(self))) {
        putCanInNormalMode(self);
        return error;
    }

    CANConfigureChannelForRx(self->module, channel, channelMsgSize, dataOnly);

    CANConfigureFilter(self->module, CAN_FILTER0, 0, CAN_SID);
    CANConfigureFilterMask(self->module, CAN_FILTER_MASK0, 0, CAN_SID, CAN_FILTER_MASK_ANY_TYPE);
    CANLinkFilterToChannel(self->module, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
    CANEnableFilter(self->module, CAN_FILTER0, TRUE);
    
    if (interruptEvents)
        CANEnableChannelEvent(self->module, channel, interruptEvents, TRUE);
    if ((error = putCanInNormalMode(self)) < 0)
        return error;

    return 0;
}


static int32_t
CANTx (const struct can *self, CAN_CHANNEL channel, enum IDType IDTypeExtended,
        uint16_t sid, uint32_t eid, uint32_t rtr, const void *data, size_t len)
{
    int32_t err;

    CANTxMessageBuffer *message = CANGetTxMessageBuffer(self->module, channel);

    if (!self || !message || (!data && len > 0))
        return -ENULPTR;

    if (len > 8)
        return -EINVAL;

    if ((err = putCanInNormalMode(self)) < 0)
        return err;

    /* clear message data */
    memset(message, 0, sizeof(CANTxMessageBuffer));

    /* insert SID/EID information */
    message->msgEID.IDE = IDTypeExtended;
    message->msgSID.SID = BITFIELD_CAST(sid, 11);  /* 11   bits */
    message->msgEID.DLC = BITFIELD_CAST(len, 4);   /* 4    bits */
    message->msgEID.RTR = BITFIELD_CAST(rtr, 1);   /* 1    bit; 1 = remote transmission request enabled */
    if (IDTypeExtended) /* EID is indicated by IDTypeExtended = 1 */
        message->msgEID.EID = BITFIELD_CAST(eid, 18);    /* 18 bits */

    if (len)
        memcpy(message->data, (const BYTE*)data, len);

    CANUpdateChannel(self->module, channel);

    CANFlushTxChannel(self->module, channel);

    return 0;
}


static long
CANRx (const struct can *self, CAN_CHANNEL channel, unsigned int *id, void *dst)
{
    CANRxMessageBuffer *message;
    unsigned long len;

    if (!self || !id || !dst)
        return -ENULPTR;

    message = (CANRxMessageBuffer *)CANGetRxMessage(self->module, channel);
    if (message == NULL)
        return -ENODATA;

    if (message->msgEID.IDE == STANDARD_ID)
        *id = (int)(message->msgSID.SID);
    else /* EXTENDED_ID */
        /* msgEID.EID is 18 bits; msgSID.SID is 11 bits */
        *id = (unsigned int)(message->msgEID.EID) | (unsigned int)(message->msgSID.SID << 18);

    len = (unsigned long) message->msgEID.DLC;

    memcpy(dst, message->data, len);

    CANUpdateChannel(self->module, channel);

    return (long) len;
}

static int32_t
addFilter(const struct can *self, CAN_CHANNEL chn,
            CAN_FILTER filter, CAN_ID_TYPE filterType,
            uint32_t id, CAN_FILTER_MASK mask,
            CAN_FILTER_MASK_TYPE mide, uint32_t maskBits)
{
    int32_t err;

    if (!self)
        return -ENULPTR;

    if ((err = putCanInConfigMode(self))) {
        putCanInNormalMode(self);
        return err;
    }

    CANConfigureFilter(self->module, filter, id, filterType);
    CANConfigureFilterMask(self->module, mask, maskBits, filterType, mide);
    CANLinkFilterToChannel(self->module, filter, mask, chn);
    CANEnableFilter(self->module, filter, TRUE);

    if ((err = putCanInNormalMode(self)) < 0)
        return err;

    return 0;
}