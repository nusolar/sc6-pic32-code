#ifndef NU_CAN_WS20_H
#define NU_CAN_WS20_H

#include "can_addresses.h"
#include "can_ws20_rx.h"
#include "can_ws20_tx.h"

union can_ws20 {
    union can_ws20_rx rx;
    union can_ws20_tx tx;
};

#endif
