#ifndef NU_CAN_ADD_CHANNEL_RX_H
#define NU_CAN_ADD_CHANNEL_RX_H 1

#include "nu/can.h"
#include "nu/types.h"

struct nu_can;
struct nu_can_add_channel_rx_attr;

s32
nu_can_add_channel_rx(const struct nu_can *c, CAN_CHANNEL chn, u32 channel_msg_size,
                    CAN_RX_DATA_MODE data_only, CAN_CHANNEL_EVENT interrupt_events);

#endif
