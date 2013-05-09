#include "can/setup.h"
#include "can.h"

struct nu_can_attr {
    u32 bus_speed_hz;
    CAN_BIT_CONFIG timings;
    CAN_MODULE_EVENT interrupt_events;
    INT_PRIORITY int_priority;
    CAN_MODULE_FEATURES features;
    void *buf;
    size_t buf_size;
};

static void
nu_can_int_setup(CAN_MODULE mod, CAN_MODULE_EVENT events,
                 INT_PRIORITY priority) {
    INT_VECTOR int_vec;
    INT_SOURCE int_src;

    if (!events)
        return;

    CANEnableModuleEvent(mod, events, TRUE);

    switch (mod) {
    case CAN1:
        int_vec = INT_CAN_1_VECTOR;
        int_src = INT_CAN1;
        break;
    case CAN2:
        int_vec = INT_CAN_2_VECTOR;
        int_src = INT_CAN2;
        break;
    case CAN_NUMBER_OF_MODULES:
    default:
        return;
    }

    INTSetVectorPriority(int_vec, priority);
    INTSetVectorSubPriority(int_vec, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(int_src, INT_ENABLED);
}

void
nu_can_setup(const struct nu_can *c, const struct nu_can_attr *a)
{
    CANEnableModule(c->module, TRUE);
    CANSetSpeed(c->module, &(a->timings), HZ, a->bus_speed_hz);
    CANAssignMemoryBuffer(c->module, a->buf, a->buf_size);
    nu_can_int_setup(c->module, a->interrupt_events, a->int_priority);

    CANSetOperatingMode(c->module, CAN_CONFIGURATION);
    while (CAN_CONFIGURATION != CANGetOperatingMode(c->module))
        Nop();

    CANEnableFeature(c->module, a->features, TRUE);

    CANSetOperatingMode(c->module, CAN_NORMAL_OPERATION);
    while (CAN_NORMAL_OPERATION != CANGetOperatingMode(c->module))
        Nop();
}
