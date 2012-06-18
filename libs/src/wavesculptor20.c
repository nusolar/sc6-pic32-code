#include "../include/wavesculptor20.h"

static int32_t
driveCmd(const struct wavesculptor20 *self, float motorCurrentPercent,
            float motorVelocityMperS);
            
static int32_t
powerCmd(const struct wavesculptor20 *self, float busCurrent);

static int32_t
resetCmd(const struct wavesculptor20 *self);

static int32_t
sendIdFrame(const struct wavesculptor20 *self);

static const struct vtbl_wavesculptor20 wavesculptor20_ops = {
    .driveCmd       = &driveCmd,
    .powerCmd       = &powerCmd,
    .resetCmd       = &resetCmd,
    .sendIdFrame    = &sendIdFrame,
};

static int32_t
wavesculptor20_init(struct wavesculptor20 *self)
{
    if (self == NULL)
        return -ENULPTR;
    
    return 0;
}

int32_t
wavesculptor20_new(struct wavesculptor20 *self,
                    uint32_t driverControlsSerialNo,
                    uint16_t driverControlsBaseAddr,
                    uint16_t motorControllerBaseAddr, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn,
                    uint32_t canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
                    CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                    enum PhaseSeg2TimeSelect selectAutoSet,
                    enum SampleTimes sample3Times, CAN_BIT_TQ syncJumpWidth)
{
    int32_t err = 0;

    if (self == NULL)
        return -ENULPTR;
    
    self->op = &wavesculptor20_ops;
    self->driverControlsSerialNo = driverControlsSerialNo;
    self->driverControlsBaseAddr = driverControlsBaseAddr;
    self->motorControllerBaseAddr = motorControllerBaseAddr;
    self->txChn = txChn;
    self->rxChn = rxChn;

    if ((err = can_new(&(self->can), module, canBusSpeedHz, phaseSeg2Tq, phaseSeg1Tq,
            propSegTq, selectAutoSet, sample3Times, syncJumpWidth, 0,
            INT_PRIORITY_DISABLED, 0)) < 0)
        return err;

    if ((err = self->can.op->addChannelTx(&(self->can), self->txChn,
                                CAN_TX_RX_MESSAGE_SIZE_BYTES,
                                CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY,
                                0)) < 0)
        return err;

    if ((err = self->can.op->addChannelRx(&(self->can), self->rxChn,
                                CAN_TX_RX_MESSAGE_SIZE_BYTES,
                                CAN_RX_FULL_RECEIVE, 0)) < 0)
        return err;

    return wavesculptor20_init(self);
}

int32_t
wavesculptor20_new_easy(struct wavesculptor20 *self,
                    CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn)
{
    if (self == NULL)
        return -ENULPTR;

    return wavesculptor20_new(self, module, 1, ADDR_WS20RX(BASE),
            ADDR_WS20TX(BASE), txChn, rxChn,
            WAVESCULPTOR20_BUS_SPEED_DEFAULT, CAN_BIT_3TQ, CAN_BIT_5TQ,
            CAN_BIT_1TQ, AUTO_SET, THREE_TIMES, CAN_BIT_1TQ);
}

static int32_t
txFrame(const struct wavesculptor20 *self, const void *src, size_t siz,
            uint32_t addr)
{
    if (self == NULL || src == NULL)
        return -ENULPTR;

    return self->can.op->tx(&(self->can), self->txChn, STANDARD_ID,
                        addr,
                        0, 0, src, siz);
}

static int32_t
driveCmd(const struct wavesculptor20 *self, float motorCurrentPercent,
            float motorVelocityMperS)
{
    if (self == NULL)
        return -ENULPTR;

    if (motorCurrentPercent < 0 || motorCurrentPercent > 1)
        return -EINVAL;

    struct CAN_WS20RX(DRIVE_CMD) frame = {
        .motorCurrent = motorCurrentPercent,
        .motorVelocity = motorVelocityMperS,
    };

    return txFrame(self, &frame, sizeof(frame), ADDR_WS20RX(DRIVE_CMD));
}

static int32_t
powerCmd(const struct wavesculptor20 *self, float busCurrent)
{
    if (self == NULL)
        return -ENULPTR;

    struct CAN_WS20RX(POWER_CMD) frame = {
        .busCurrent = busCurrent,
        .reserved = 0,
    };

    return txFrame(self, &frame, sizeof(frame), ADDR_WS20RX(POWER_CMD));
}

static int32_t
resetCmd(const struct wavesculptor20 *self)
{
    if (self == NULL)
        return -ENULPTR;

    struct CAN_WS20RX(RESET_CMD) frame = {0};

    return txFrame(self, &frame, sizeof(frame), ADDR_WS20RX(RESET_CMD));
}

static int32_t
sendIdFrame(const struct wavesculptor20 *self)
{
    if (self == NULL)
        return -ENULPTR;

    struct CAN_WS20RX(DRIVER_CONTROLS_ID) frame = {
        .drvId = "TRIb",
        .serialNo = self->driverControlsSerialNo,
    };

    return txFrame(self, &frame, sizeof(frame), ADDR_WS20RX(DRIVER_CONTROLS_ID));
}
