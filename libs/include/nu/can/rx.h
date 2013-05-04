#ifndef NU_CAN_RX_H
#define NU_CAN_RX_H 1

#include "nu/types.h"
#include <peripheral/CAN.h>

struct nu_can;
struct nu_can_rx_attr;

s64
nu_can_rx(const struct nu_can *c, const struct nu_can_rx_attr *a,
          u32 *id, void *dst);

s32
nu_can_rx_attr_init(struct nu_can_rx_attr *a);

s32
nu_can_rx_attr_destroy(struct nu_can_rx_attr *a);

s32
nu_can_rx_attr_get_chn(const struct nu_can_rx_attr *a, CAN_CHANNEL *chn);

s32
nu_can_rx_attr_set_chn(struct nu_can_rx_attr *a, CAN_CHANNEL chn);

#endif
