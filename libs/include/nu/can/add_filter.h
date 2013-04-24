#ifndef NU_CAN_ADD_FILTER_H
#define NU_CAN_ADD_FILTER_H 1

#include "nu/nu_types.h"
#include <peripheral/CAN.h>

struct nu_can;
struct nu_can_add_filter_attr;

s32
nu_can_add_filter(const struct nu_can *c, CAN_CHANNEL chn, CAN_FILTER filter,
                CAN_ID_TYPE filter_type, u32 id, CAN_FILTER_MASK mask,
                CAN_FILTER_MASK_TYPE mide, u32 mask_bits);

#endif
