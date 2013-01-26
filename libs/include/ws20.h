/* wavesculptor 20 */

#ifndef NU_WS20_H
#define NU_WS20_H

#include "can.h"
#include "can_all.h"
#include "errorcodes.h"
#include "nu_types.h"

struct ws20 {
    struct can can;
    u32 driver_controls_serial_no;
    struct chn {
        CAN_CHANNEL tx;
        CAN_CHANNEL rx;
    } chn;
    struct base_addr {
        u16 driver_controls;
        u16 motor_controller;
    } base_addr;
};

#define WS20_INIT(can_mod, serial_no, tx_chn, rx_chn, base_driver_controls, base_motor_controller)  \
    {.can = CAN_INIT(can_mod), .driver_controls_serial_no = (serial_no),  \
    .chn = {.tx = (tx_chn), .rx = (rx_chn)}, \
    .base_addr = {.driver_controls = (base_driver_controls), .motor_controller = (base_motor_controller)}}

static inline void
INIT_WS20(struct ws20 *w, )

s32
ws20_send_id_frame(const struct ws20 *w);

s32
ws20_drive_cmd(const struct ws20 *w, float motor_current_percent,
          float motor_velocity_m_per_s);

s32
ws20_power_cmd(const struct ws20 *w, float bus_current);

s32
ws20_reset_cmd(const struct ws20 *w);

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
