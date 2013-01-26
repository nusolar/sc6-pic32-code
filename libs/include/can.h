#ifndef NU_CAN_H
#define NU_CAN_H

#include <peripheral/CAN.h>
#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"

enum id_type {
    STANDARD_ID,
    EXTENDED_ID
};

enum phase_seg2_time_select {
    MAN_SET,
    AUTO_SET
};

enum sample_times {
    ONE_TIME,
    THREE_TIMES
};

struct can {
    struct error_reporting_dev  erd;
    CAN_CHANNEL                 erd_chn;
    enum id_type                erd_id_type;
    u16                         erd_std_id;
    u16                         erd_ext_id;

    CAN_MODULE module;
    /* max of 32 channels, each with a 32-message buffer */
    byte buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];
};

#define erd_to_can(erdp)    container_of((erdp), struct can, erd)

extern const struct vtbl_error_reporting_dev can_erd_ops;

#define CAN_ERD_INIT(min_priority, _erd_chn, _erd_id_type, _erd_std_id, _erd_ext_id, mod)   \
    {                                           \
    .erd = ERD_INIT(can_erd_ops, min_priority), \
    .erd_chn = (_erd_chn),                      \
    .erd_id_type = (_erd_id_type),              \
    .erd_std_id = (_erd_std_id),                \
    .erd_ext_id = (_erd_ext_id),                \
    .module = (mod)                             \
    }
#define CAN_ERD(name, min_priority, _erd_chn, _erd_id_type, _erd_std_id, _erd_ext_id, mod)  \
    struct can name = CAN_ERD_INIT(name, min_priority, _erd_chn, _erd_id_type, _erd_std_id, _erd_ext_id, mod)
#define CAN_INIT(mod)   { .module = (mod) }
#define CAN(name, mod)    \
    struct can name = CAN_INIT(mod)

s32
can_tx(const struct can *c, CAN_CHANNEL chn, enum id_type id_type, u16 sid, u32 eid,
        u32 rtr, const void *src, size_t n);

s64
can_rx(const struct can *c, CAN_CHANNEL chn, u32 *id, void *dst);

s32
can_add_channel_tx(const struct can *c, CAN_CHANNEL chn, u32 channel_msg_size,
                    CAN_TX_RTR rtr_enabled, CAN_TXCHANNEL_PRIORITY prioritiy,
                    CAN_CHANNEL_EVENT interrupt_events);

s32
can_add_channel_rx(const struct can *c, CAN_CHANNEL chn, u32 channel_msg_size,
                    CAN_RX_DATA_MODE data_only, CAN_CHANNEL_EVENT interrupt_events);

s32
can_add_filter(const struct can *c, CAN_CHANNEL chn, CAN_FILTER filter,
                CAN_ID_TYPE filter_type, u32 id, CAN_FILTER_MASK mask,
                CAN_FILTER_MASK_TYPE mide, u32 mask_bits);

s32
can_new(struct can *c, CAN_MODULE mod, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features);

int32_t
can_new_easy (struct can *self, CAN_MODULE module,
                CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority);

COLD s32
can_report(struct error_reporting_dev *erd,
    const char *file, u32 line, const char *expr,
    enum report_priority priority, s32 err, const char *err_name,
    const char *fmtd_msg)

#endif
