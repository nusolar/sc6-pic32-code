#ifndef NU_CAN_RX_H
#define NU_CAN_RX_H 1

#include "nu/types.h"
#include <peripheral/CAN.h>

struct nu__Can;
struct nu__Can__RxAttr;

s64
nu__Can__rx(const struct nu__Can *c, const struct nu__Can__RxAttr *a,
          u32 *id, void *dst);

s32
nu__Can__rx_attr_init(struct nu__Can__RxAttr *a);

s32
nu__Can__rx_attr_destroy(struct nu__Can__RxAttr *a);

s32
nu__Can__rx_attr_get_chn(const struct nu__Can__RxAttr *a, CAN_CHANNEL *chn);

s32
nu__Can__rx_attr_set_chn(struct nu__Can__RxAttr *a, CAN_CHANNEL chn);

#endif
