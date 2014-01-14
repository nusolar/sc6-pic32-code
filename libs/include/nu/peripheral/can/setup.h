#ifndef NU_CAN_SETUP_H
#define NU_CAN_SETUP_H 1

#include "nu/types.h"
#include <peripheral/CAN.h>
#include <peripheral/int.h>

enum nu__Can__id_type {
    NU_CAN_STANDARD_ID,
    NU_CAN_EXTENDED_ID
};

enum nu__Can__phase_seg2_time_select {
    NU_CAN_PHASESEG2_MANUAL,
    NU_CAN_PHASESEG2_AUTO
};

enum nu__Can__sample_times {
    NU_CAN_SAMPLE_ONE_TIME,
    NU_CAN_SAMPLE_THREE_TIMES
};

struct nu__Can;
struct nu__Can__Attr;

/* null attribute object means default settings */
void
nu__Can__setup(const struct nu__Can *c, const struct nu__Can__Attr *a);

s32
nu__Can__attr_init(struct nu__Can__Attr *a);

s32
nu__Can__attr_destroy(struct nu__Can__Attr *a);

s32
nu__Can__attr_get_bus_speed(const struct nu__Can__Attr *a, u32 *bus_speed_hz);

s32
nu__Can__attr_set_bus_speed(struct nu__Can__Attr *a, u32 bus_speed_hz);

s32
nu__Can__attr_get_timings(const struct nu__Can__Attr *a, CAN_BIT_CONFIG *timings);

s32
nu__Can__attr_set_timings(struct nu__Can__Attr *a, CAN_BIT_CONFIG timings);

s32
nu__Can__attr_get_interrupt_events(const struct nu__Can__Attr *a, CAN_MODULE_EVENT *interrupt_events);

s32
nu__Can__attr_set_interrupt_events(struct nu__Can__Attr *a, CAN_MODULE_EVENT interrupt_events);

s32
nu__Can__attr_get_interrupt_priority(const struct nu__Can__Attr *a, INT_PRIORITY *priority);

s32
nu__Can__attr_set_interrupt_priority(struct nu__Can__Attr *a, INT_PRIORITY priority);

s32
nu__Can__attr_get_features(const struct nu__Can__Attr *a, CAN_MODULE_FEATURES *features);

s32
nu__Can__attr_set_features(struct nu__Can__Attr *a, CAN_MODULE_FEATURES features);

s32
nu__Can__attr_get_buffer(const struct nu__Can__Attr *a, void **buf);

s32
nu__Can__attr_set_buffer(struct nu__Can__Attr *a, void *buf);

s32
nu__Can__attr_get_buffer_size(const struct nu__Can__Attr *a, size_t *buf_size);

s32
nu__Can__attr_set_buffer_size(struct nu__Can__Attr *a, size_t buf_size);

#endif
