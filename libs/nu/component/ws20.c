#include "nu/component/ws20.h"
#include "nu/errorcodes.h"

int32_t
nu_ws20_init(struct nu_ws20 *self);
int32_t
tx_frame(const struct nu_ws20 *self, const void *src, size_t siz, uint32_t addr);

static const struct vtbl_wavesculptor20 wavesculptor20_ops = {
    .driveCmd       = &nu_ws20_drive_cmd,
    .powerCmd       = &nu_ws20_power_cmd,
    .resetCmd       = &nu_ws20_reset_cmd,
    .sendIdFrame    = &nu_ws20_send_id_frame,
};

int32_t
nu_ws20_init(struct nu_ws20 *self)
{
    if (self == NULL)
        return -ENULPTR;

    return 0;
}

int32_t
nu_ws20_new(struct nu_ws20 *self,
                    uint32_t driverControlsSerialNo,
                    uint16_t driverControlsBaseAddr,
                    uint16_t motorControllerBaseAddr, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn,
                    uint32_t canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
                    CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                    enum nu_can_phase_seg2_time_select selectAutoSet,
                    enum nu_can_sample_times sample3Times, CAN_BIT_TQ syncJumpWidth)
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
                                32,
                                CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY,
                                0)) < 0)
        return err;

    if ((err = self->can.op->addChannelRx(&(self->can), self->rxChn,
                                32,
                                CAN_RX_FULL_RECEIVE, 0)) < 0)
        return err;

    return nu_ws20_init(self);
}

int32_t
nu_ws20_new_easy(struct nu_ws20 *self,
                    CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn)
{
    if (self == NULL)
        return -ENULPTR;

    return nu_ws20_new(self, module, 1, ADDR_WS20_RX_BASE,
            ADDR_WS20_TX_BASE, txChn, rxChn,
            WAVESCULPTOR20_BUS_SPEED_DEFAULT, CAN_BIT_3TQ, CAN_BIT_5TQ,
            CAN_BIT_1TQ, AUTO_SET, THREE_TIMES, CAN_BIT_1TQ);
}

int32_t
tx_frame(const struct nu_ws20 *self, const void *src, size_t siz,
            uint32_t addr)
{
    if (self == NULL || src == NULL)
        return -ENULPTR;

    return self->can.op->tx(&(self->can), self->txChn, STANDARD_ID,
                        addr,
                        0, 0, src, siz);
}

int32_t
nu_ws20_drive_cmd(const struct nu_ws20 *self, float motorCurrentPercent,
            float motorVelocityMperS)
{
    if (self == NULL)
        return -ENULPTR;

    if (motorCurrentPercent < 0 || motorCurrentPercent > 1)
        return -EINVAL;

    struct can_ws20_rx_drive_cmd frame = {
        .motorCurrent = motorCurrentPercent,
        .motorVelocity = motorVelocityMperS,
    };

    return tx_frame(self, &frame, sizeof(frame), ADDR_WS20_RX_DRIVE_CMD);
}

int32_t
nu_ws20_power_cmd(const struct nu_ws20 *self, float busCurrent)
{
    if (self == NULL)
        return -ENULPTR;

    struct can_ws20_rx_power_cmd frame = {
        .busCurrent = busCurrent,
        .reserved = 0,
    };

    return tx_frame(self, &frame, sizeof(frame), ADDR_WS20_RX_POWER_CMD);
}

int32_t
nu_ws20_reset_cmd(const struct nu_ws20 *self)
{
    if (self == NULL)
        return -ENULPTR;

    struct can_ws20_rx_reset_cmd frame = {0};

    return tx_frame(self, &frame, sizeof(frame), ADDR_WS20_RX_RESET_CMD);
}

int32_t
nu_ws20_send_id_frame(const struct nu_ws20 *self)
{
    if (self == NULL)
        return -ENULPTR;

    struct can_ws20_rx_driver_controls_id frame = {
        .drvId = "TRIb",
        .serialNo = self->driverControlsSerialNo,
    };

    return tx_frame(self, &frame, sizeof(frame), ADDR_WS20_RX_DRIVER_CONTROLS_ID);
}
