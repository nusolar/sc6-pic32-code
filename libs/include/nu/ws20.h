/* wavesculptor 20 */

#ifndef NU_WS20_H
#define NU_WS20_H 1

#include "nu/can.h"
/*#include "can_all.h"*/
#include "nu/can_addresses.h"
#include "nu/types.h"

struct nu_ws20 {
    struct nu_can can;
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
    struct nu_ws20 name = WS20_INIT(can_mod, serial_no, tx_chn, rx_chn, base_driver_controls, base_motor_controller)

s32
nu_ws20_send_id_frame(const struct nu_ws20 *w);

s32
nu_ws20_drive_cmd(const struct nu_ws20 *w, float motor_current_percent,
          float motor_velocity_m_per_s);

s32
nu_ws20_power_cmd(const struct nu_ws20 *w, float bus_current);

s32
nu_ws20_reset_cmd(const struct nu_ws20 *w);

#define WAVESCULPTOR20_BUS_SPEED_DEFAULT    (1000000) /* 1 Mbit/s */

void
nu_ws20_setup(struct nu_ws20 *w, u32 bus_speed_hz, CAN_BIT_CONFIG *cfg);

s32
nu_ws20_new(struct nu_ws20 *w, u32 driverControlsSerialNo,
                    u16 driverControlsBaseAddr,
                    u16 motorControllerBaseAddr, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn,
                    u32 canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
                    CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
                    enum nu_phase_seg2_time_select selectAutoSet,
                    enum nu_sample_times sample3Times, CAN_BIT_TQ syncJumpWidth);

s32
nu_ws20_new_easy(struct nu_ws20 *self, CAN_MODULE module,
                    CAN_CHANNEL txChn, CAN_CHANNEL rxChn);

#endif
