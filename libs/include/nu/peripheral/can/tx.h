#ifndef NU_CAN_TX_H
#define NU_CAN_TX_H 1

#include "nu/peripheral/can/setup.h"
#include "nu/types.h"
#include <peripheral/CAN.h>

struct nu__Can;
struct nu__Can__TxAttr;

s32
nu__Can__tx(const struct nu__Can *c, const struct nu__Can__TxAttr *a,
          const void *src, size_t n);

s32
nu__Can__tx_attr_init(struct nu__Can__TxAttr *a);

s32
nu__Can__tx_attr_destroy(struct nu__Can__TxAttr *a);

s32
nu__Can__tx_attr_get_chn(const struct nu__Can__TxAttr *a, CAN_CHANNEL *chn);

s32
nu__Can__tx_attr_set_chn(struct nu__Can__TxAttr *a, CAN_CHANNEL chn);

s32
nu__Can__tx_attr_get_id_type(const struct nu__Can__TxAttr *a,
                           enum nu__Can__id_type *type);

s32
nu__Can__tx_attr_set_id_type(struct nu__Can__TxAttr *a,
                           enum nu__Can__id_type type);

s32
nu__Can__tx_attr_get_sid(const struct nu__Can__TxAttr *a, u16 *sid);

s32
nu__Can__tx_attr_set_sid(struct nu__Can__TxAttr *a, u16 sid);

s32
nu__Can__tx_attr_get_eid(const struct nu__Can__TxAttr *a, u32 *eid);

s32
nu__Can__tx_attr_set_eid(struct nu__Can__TxAttr *a, u32 eid);

s32
nu__Can__tx_attr_get_rtr(const struct nu__Can__TxAttr *a, bool *rtr);

s32
nu__Can__tx_attr_set_rtr(struct nu__Can__TxAttr *a, bool rtr);

#endif
