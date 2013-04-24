#ifndef NU_CAN_ADD_CHANNEL_TX_H
#define NU_CAN_ADD_CHANNEL_TX_H 1

#include "nu/nu_types.h"
#include "nu/can.h"

struct nu_can;
struct nu_can_add_channel_tx_attr;

void
nu_can_add_channel_tx(const struct nu_can *c,
                      const struct nu_can_add_channel_tx_attr *a);

#endif
