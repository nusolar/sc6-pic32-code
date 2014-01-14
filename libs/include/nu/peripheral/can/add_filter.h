#ifndef NU_CAN_ADD_FILTER_H
#define NU_CAN_ADD_FILTER_H 1

#include "nu/types.h"
#include <peripheral/CAN.h>

struct nu__Can;
struct nu__Can__AddFilterAttr;

s32
nu__Can__add_filter(const struct nu__Can *c, CAN_CHANNEL chn, CAN_FILTER filter,
                CAN_ID_TYPE filter_type, u32 id, CAN_FILTER_MASK mask,
                CAN_FILTER_MASK_TYPE mide, u32 mask_bits);

#endif
