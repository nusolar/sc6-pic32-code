#ifndef NU_CAN_ADD_CHANNEL_TX_H
#define NU_CAN_ADD_CHANNEL_TX_H 1

#include "nu/peripheral/can.h"
#include "nu/types.h"

struct nu__Can;
struct nu__Can__AddChannelTxAttr;

void
nu__Can__add_channel_tx(const struct nu__Can *c,
                      const struct nu__Can__AddChannelTxAttr *a);

#endif
