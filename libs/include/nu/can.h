#ifndef NU_CAN_H
#define NU_CAN_H 1

#include "nu/can/setup.h"
#include "nu/can/rx.h"
#include "nu/can/tx.h"
#include "nu/can/add_channel_rx.h"
#include "nu/can/add_channel_tx.h"
#include "nu/can/add_filter.h"

#include "nu/nu_types.h"
#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include <peripheral/CAN.h>
#include <peripheral/int.h>

extern const struct nu_vtbl_error_reporting_dev nu_can_erd_ops;

struct nu_can {
    struct nu_error_reporting_dev  erd;
#define nu_erd_to_can(erdp) container_of((erdp), struct nu_can, erd)
    CAN_CHANNEL erd_chn;
    enum nu_can_id_type erd_id_type;
    u16 erd_std_id;
    u16 erd_ext_id;

    CAN_MODULE module;
    /* max of 32 channels, each with a 32-message buffer */
    /* byte buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES]; */
};
#define NU_CAN_ERD_INIT(min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod, buf)   \
    { \
    NU_ERD_INIT(nu_can_erd_ops, min_priority), \
    (erd_chn), \
    (erd_id_type), \
    (erd_std_id), \
    (erd_ext_id), \
    (mod) \
    }
#define NU_CAN_INIT(mod) \
    NU_CAN_ERD_INIT(NU_REP_DEBUG, CAN_CHANNEL0, STANDARD_ID, 0, 0, mod)
#define NU_CAN_ERD(name, min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod) \
    struct nu_can name = NU_CAN_ERD_INIT(min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod)
#define NU_CAN(name, mod) \
    struct nu_can name = NU_CAN_INIT(mod)

/*
s32
nu_can_new(struct nu_can *c, CAN_MODULE mod, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features);

s32
nu_can_new_easy (struct nu_can *self, CAN_MODULE module,
                CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority);

COLD s32
nu_can_report(struct nu_error_reporting_dev *erd,
    const char *file, u32 line, const char *expr,
    enum nu_report_priority priority, s32 err, const char *err_name,
    const char *fmtd_msg);
 */

#endif

