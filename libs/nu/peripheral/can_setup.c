#include "nu/peripheral/can/setup.h"
#include "nu/peripheral/can.h"
#include "nu/param.h"

struct nu__Can__Attr {
    u32 bus_speed_hz;
    CAN_BIT_CONFIG timings;
    CAN_MODULE_EVENT interrupt_events;
    INT_PRIORITY int_priority;
    CAN_MODULE_FEATURES features;
    void *buf;
    size_t buf_size;
};

static void
nu__Can__int_setup(CAN_MODULE mod, CAN_MODULE_EVENT events,
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
nu__Can__setup(const struct nu__Can *c, const struct nu__Can__Attr *a)
{
    CANEnableModule(c->module, TRUE);
    CANSetSpeed(c->module, &(a->timings), NU_HZ, a->bus_speed_hz);
    CANAssignMemoryBuffer(c->module, a->buf, a->buf_size);
    nu__Can__int_setup(c->module, a->interrupt_events, a->int_priority);

    CANSetOperatingMode(c->module, CAN_CONFIGURATION);
    while (CAN_CONFIGURATION != CANGetOperatingMode(c->module))
        Nop();

    CANEnableFeature(c->module, a->features, TRUE);

    CANSetOperatingMode(c->module, CAN_NORMAL_OPERATION);
    while (CAN_NORMAL_OPERATION != CANGetOperatingMode(c->module))
        Nop();
}
