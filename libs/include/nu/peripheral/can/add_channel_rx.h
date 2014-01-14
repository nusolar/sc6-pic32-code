#ifndef NU_CAN_ADD_CHANNEL_RX_H
#define NU_CAN_ADD_CHANNEL_RX_H 1

#include "nu/peripheral/can.h"
#include "nu/types.h"

struct nu__Can;
struct nu__Can__AddChannelRxAttr;

s32
nu__Can__add_channel_rx(const struct nu__Can *c, CAN_CHANNEL chn, u32 channel_msg_size,
                    CAN_RX_DATA_MODE data_only, CAN_CHANNEL_EVENT interrupt_events);

#endif
