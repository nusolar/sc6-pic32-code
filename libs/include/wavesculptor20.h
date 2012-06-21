#ifndef __NU_WAVESCULPTOR20_H
#define __NU_WAVESCULPTOR20_H

#include <stdint.h>
#include "can.h"
#include "errorcodes.h"
#include "can_nu.h"

struct wavesculptor20 {
    struct can      can;
    
    const struct vtbl_wavesculptor20 *op;

    uint32_t        driverControlsSerialNo;
    
    CAN_CHANNEL     txChn;
    CAN_CHANNEL     rxChn;
    
    uint16_t        driverControlsBaseAddr;
    uint16_t        motorControllerBaseAddr;
};

struct vtbl_wavesculptor20 {    
    /* DRIVE COMMANDS */
    int32_t     (*sendIdFrame)          (const struct wavesculptor20 *self);
    int32_t     (*driveCmd)             (const struct wavesculptor20 *self,
                                            float motorCurrentPercent,
                                            float motorVelocityMperS);
    int32_t     (*powerCmd)             (const struct wavesculptor20 *self,
                                            float busCurrent);
    int32_t     (*resetCmd)             (const struct wavesculptor20 *self);
};

#define WAVESCULPTOR20_BUS_SPEED_DEFAULT    (1000000) /* 1 Mbit/s */

int32_t
wavesculptor20_new(struct wavesculptor20 *self, uint32_t driverControlsSerialNo,
                    uint16_t driverControlsBaseAddr,
                    uint16_t motorControllerBaseAddr, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn,
                    uint32_t canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
                    CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                    enum PhaseSeg2TimeSelect selectAutoSet,
                    enum SampleTimes sample3Times, CAN_BIT_TQ syncJumpWidth);

int32_t
wavesculptor20_new_easy(struct wavesculptor20 *self, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn);

#endif
