#ifndef NU_CAN_H
#define NU_CAN_H 1

#include "nu/peripheral/can/setup.h"
#include "nu/peripheral/can/rx.h"
#include "nu/peripheral/can/tx.h"
#include "nu/peripheral/can/add_channel_rx.h"
#include "nu/peripheral/can/add_channel_tx.h"
#include "nu/peripheral/can/add_filter.h"

#include "nu/types.h"
#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include <peripheral/CAN.h>
#include <peripheral/int.h>

extern const struct nu__ErrorReportingDev__Vtbl nu__Can__erd_ops;
#define nu_erd_to_can(erdp) container_of((erdp), struct nu__Can, erd)

struct nu__Can {
    struct nu__ErrorReportingDev  erd;
    CAN_CHANNEL erd_chn;
    enum nu__Can__id_type erd_id_type;
    u16 erd_std_id;
    u16 erd_ext_id;

    CAN_MODULE module;
    /* max of 32 channels, each with a 32-message buffer */
    byte buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];
};

#define NU_CAN_ERD_INIT(min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod, buf)   \
    { \
    NU_ERD_INIT(nu__Can__erd_ops, min_priority), \
    (erd_chn), \
    (erd_id_type), \
    (erd_std_id), \
    (erd_ext_id), \
    (mod) \
    }
#define NU_CAN_INIT(mod) \
    NU_CAN_ERD_INIT(NU_REP_DEBUG, CAN_CHANNEL0, STANDARD_ID, 0, 0, mod)
#define NU_CAN_ERD(name, min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod) \
    struct nu__Can name = NU_CAN_ERD_INIT(min_priority, erd_chn, erd_id_type, erd_std_id, erd_ext_id, mod)
#define NU_CAN(name, mod) \
    struct nu__Can name = NU_CAN_INIT(mod)

/*
s32
nu__Can__new(struct nu__Can *c, CAN_MODULE mod, u32 bus_speed_hz, CAN_BIT_CONFIG *timings,
        CAN_MODULE_EVENT interrupt_events, INT_PRIORITY int_priority,
        CAN_MODULE_FEATURES features);

s32
nu__Can__new_easy (struct nu__Can *self, CAN_MODULE module,
                CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority);

COLD s32
nu__Can__report(struct nu__ErrorReportingDev *erd,
    const char *file, u32 line, const char *expr,
    enum nu_report_priority priority, s32 err, const char *err_name,
    const char *fmtd_msg);
 */

#endif

