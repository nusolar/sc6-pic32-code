#ifndef NU_CAN_SETUP_H
#define NU_CAN_SETUP_H 1

#include "nu/nu_types.h"
#include <peripheral/CAN.h>
#include <peripheral/int.h>

enum nu_can_id_type {
    NU_CAN_STANDARD_ID,
    NU_CAN_EXTENDED_ID
};

enum nu_can_phase_seg2_time_select {
    NU_CAN_PHASESEG2_MANUAL,
    NU_CAN_PHASESEG2_AUTO
};

enum nu_can_sample_times {
    NU_CAN_SAMPLE_ONE_TIME,
    NU_CAN_SAMPLE_THREE_TIMES
};

struct nu_can;
struct nu_can_attr;

/* null attribute object means default settings */
void
nu_can_setup(const struct nu_can *c, const struct nu_can_attr *a);

s32
nu_can_attr_init(struct nu_can_attr *a);

s32
nu_can_attr_destroy(struct nu_can_attr *a);

s32
nu_can_attr_get_bus_speed(const struct nu_can_attr *a, u32 *bus_speed_hz);

s32
nu_can_attr_set_bus_speed(struct nu_can_attr *a, u32 bus_speed_hz);

s32
nu_can_attr_get_timings(const struct nu_can_attr *a, CAN_BIT_CONFIG *timings);

s32
nu_can_attr_set_timings(struct nu_can_attr *a, CAN_BIT_CONFIG timings);

s32
nu_can_attr_get_interrupt_events(const struct nu_can_attr *a, CAN_MODULE_EVENT *interrupt_events);

s32
nu_can_attr_set_interrupt_events(struct nu_can_attr *a, CAN_MODULE_EVENT interrupt_events);

s32
nu_can_attr_get_interrupt_priority(const struct nu_can_attr *a, INT_PRIORITY *priority);

s32
nu_can_attr_set_interrupt_priority(struct nu_can_attr *a, INT_PRIORITY priority);

s32
nu_can_attr_get_features(const struct nu_can_attr *a, CAN_MODULE_FEATURES *features);

s32
nu_can_attr_set_features(struct nu_can_attr *a, CAN_MODULE_FEATURES features);

s32
nu_can_attr_get_buffer(const struct nu_can_attr *a, void **buf);

s32
nu_can_attr_set_buffer(struct nu_can_attr *a, void *buf);

s32
nu_can_attr_get_buffer_size(const struct nu_can_attr *a, size_t *buf_size);

s32
nu_can_attr_set_buffer_size(struct nu_can_attr *a, size_t buf_size);

#endif
