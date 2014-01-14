/* wavesculptor 20 */

#ifndef NU_WS20_H
#define NU_WS20_H 1

#include "nu/peripheral/can.h"
/*#include "can_all.h"*/
/*#include "nu/peripheral/can_addresses.h"*/
#include "nu/types.h"

struct nu__WS20 {
    struct nu__Can can;
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
    {                                                   \
    CAN_INIT(can_mod),                                  \
    (serial_no),                                        \
    {(tx_chn), (rx_chn)},                               \
    {(base_driver_controls), (base_motor_controller)}   \
    }
#define WS20(name, can_mod, serial_no, tx_chn, rx_chn, base_driver_controls, base_motor_controller) \
    struct nu__WS20 name = WS20_INIT(can_mod, serial_no, tx_chn, rx_chn, base_driver_controls, base_motor_controller)

s32
nu__WS20__send_id_frame(const struct nu__WS20 *w);

s32
nu__WS20__drive_cmd(const struct nu__WS20 *w, float motor_current_percent,
          float motor_velocity_m_per_s);

s32
nu__WS20__power_cmd(const struct nu__WS20 *w, float bus_current);

s32
nu__WS20__reset_cmd(const struct nu__WS20 *w);

#define WAVESCULPTOR20_BUS_SPEED_DEFAULT    (1000000) /* 1 Mbit/s */

void
nu__WS20__setup(struct nu__WS20 *w, u32 bus_speed_hz, CAN_BIT_CONFIG *cfg);

s32
nu__WS20__new(struct nu__WS20 *w, u32 driverControlsSerialNo,
                    u16 driverControlsBaseAddr,
                    u16 motorControllerBaseAddr, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn,
                    u32 canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
                    CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                    enum nu__Can__phase_seg2_time_select selectAutoSet,
                    enum nu__Can__sample_times sample3Times, CAN_BIT_TQ syncJumpWidth);

s32
nu__WS20__new_easy(struct nu__WS20 *self, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn);

#endif
