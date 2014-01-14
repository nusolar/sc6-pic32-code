#ifndef NU_CAN_ALL_H
#define NU_CAN_ALL_H

#include "nu/compiler.h"

#define CAN_TX_MSG_SID_INIT(sid)    {(sid), 0}
#define CAN_MSG_EID_INIT(len, rtr, eid, is_extended)    \
    {(len), 0, 0, (rtr), (eid), (is_extended), 0}
#define CANTXMESSAGEBUFFER_INIT(sid, eid, is_extended, len, rtr)    \
    {{CAN_TX_MSG_SID_INIT(sid), CAN_TX_MSG_EID_INIT(len, rtr, eid, is_extended)}}

/*  #pragma GCC diagnostic push */
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wpacked"
    #pragma GCC diagnostic ignored "-Wattributes"

/* define packet data structures */
#define CAN_FRAMEE(name, fmt)
#define CAN_FRAMEA(name, layout, fmt)           PACKED struct can_data_##name { layout };
#define CAN_FRAMEE_ID(name, id, fmt)
#define CAN_FRAMEA_ID(name, id, layout, fmt)    CAN_FRAMA(name, id, fmt)
#include "can.def"
#undef CAN_FRAMEE
#undef CAN_FRAMEA
#undef CAN_FRAMEE_ID
#undef CAN_FRAMEA_ID

/* define CAN addresses */
enum can_addresses {
#define CAN_FRAMEE(name, fmt)                   can_addr_##name,
#define CAN_FRAMEA(name, layout, fmt)           CAN_FRAMEE(name, fmt)
#define CAN_FRAMEE_ID(name, id, fmt)            can_addr_##name = id,
#define CAN_FRAMEA_ID(name, id, layout, fmt)    CAN_FRAMEE_ID(name, id, fmt)
#   include "can.def"
#undef CAN_FRAMEE
#undef CAN_FRAMEA
#undef CAN_FRAMEE_ID
#undef CAN_FRAMEA_ID
    NUM_CAN_ADDRESSES
};

/* extern declarations of const CANTxMessageBuffer's */
#define CAN_FRAMEE(name, fmt)           \
    extern const CANTxMessageBuffer can_##name_default;
#define CAN_FRAMEE_ID(name, id, fmt)    \
    CAN_FRAMEE(name, fmt)
#define CAN_FRAMEA(name, layout, fmt)   \
    CAN_FRAMEE(name, fmt)
#define CAN_FRAMEA_ID(name, id, layout, fmt)    \
    CAN_FRAMEE(name, fmt)
#include "can.def"
#undef CAN_FRAMEE
#undef CAN_FRAMEA
#undef CAN_FRAMEE_ID
#undef CAN_FRAMEA_ID

    /*  #pragma GCC diagnostic pop */
    #pragma GCC diagnostic warning "-Wmissing-field-initializers"
    #pragma GCC diagnostic warning "-Wpacked"
    #pragma GCC diagnostic warning "-Wattributes"

#endif
