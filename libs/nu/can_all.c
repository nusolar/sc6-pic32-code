#include "can_all.h"

#define CAN_FRAMEE(name, fmt)           \
    const CANTxMessageBuffer can_##name_default = CANTXMESSAGEBUFFER_INIT(can_addr_##name, 0, 0, 0, 0);
#define CAN_FRAMEE_ID(name, id, fmt)    \
    CAN_FRAMEE(name, fmt)
#define CAN_FRAMEA(name, layout, fmt)   \
    const CANTxMessageBuffer can_##name_default = CANTXMESSAGEBUFFER_INIT(can_addr_##name, 0, 0, sizeof(struct can_data_##name), 0);
#define CAN_FRAMEA_ID(name, id, layout, fmt)    \
    CAN_FRAMA(name, id, layout, fmt)
#include "can.def"
#undef CAN_FRAMEE
#undef CAN_FRAMEA
#undef CAN_FRAMEE_ID
#undef CAN_FRAMEA_ID

#define CAN_FMT         dst, n, 
#define CAN_MEMBER      c->

#define CAN_FRAMEE(name, fmt)           \
    void can_print_##name(char *dst, size_t n) { snprintf fmt; }
#define CAN_FRAMEE_ID(name, id, fmt)    \
    CAN_FRAMEE(name, fmt)
#define CAN_FRAMEA(name, layout, fmt)   \
    void can_print_##name(struct can_data_##name *c, char *dst, size_t n) { snprintf fmt; }
#define CAN_FRAMEA_ID(name, id, layout, fmt)    \
    CAN_FRAMA(name, fmt, id, layout)
#include "can.def"
#undef CAN_FRAMEE
#undef CAN_FRAMEA
#undef CAN_FRAMEE_ID
#undef CAN_FRAMEA_ID