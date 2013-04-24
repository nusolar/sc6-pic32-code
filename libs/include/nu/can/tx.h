#ifndef NU_CAN_TX_H
#define NU_CAN_TX_H 1

#include "nu/can/setup.h"
#include "nu/nu_types.h"
#include <peripheral/CAN.h>

struct nu_can;
struct nu_can_tx_attr;

s32
nu_can_tx(const struct nu_can *c, const struct nu_can_tx_attr *a,
          const void *src, size_t n);

s32
nu_can_tx_attr_init(struct nu_can_tx_attr *a);

s32
nu_can_tx_attr_destroy(struct nu_can_tx_attr *a);

s32
nu_can_tx_attr_get_chn(const struct nu_can_tx_attr *a, CAN_CHANNEL *chn);

s32
nu_can_tx_attr_set_chn(struct nu_can_tx_attr *a, CAN_CHANNEL chn);

s32
nu_can_tx_attr_get_id_type(const struct nu_can_tx_attr *a,
                           enum nu_can_id_type *type);

s32
nu_can_tx_attr_set_id_type(struct nu_can_tx_attr *a,
                           enum nu_can_id_type type);

s32
nu_can_tx_attr_get_sid(const struct nu_can_tx_attr *a, u16 *sid);

s32
nu_can_tx_attr_set_sid(struct nu_can_tx_attr *a, u16 sid);

s32
nu_can_tx_attr_get_eid(const struct nu_can_tx_attr *a, u32 *eid);

s32
nu_can_tx_attr_set_eid(struct nu_can_tx_attr *a, u32 eid);

s32
nu_can_tx_attr_get_rtr(const struct nu_can_tx_attr *a, bool *rtr);

s32
nu_can_tx_attr_set_rtr(struct nu_can_tx_attr *a, bool rtr);

#endif
