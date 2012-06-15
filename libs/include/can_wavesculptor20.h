#ifndef __NU_CAN_WAVESCULPTOR_20_H
#define __NU_CAN_WAVESCULPTOR_20_H

#include "can_addresses.h"
#include "can_wavesculptor_broadcast_messages.h"
#include "can_wavesculptor_drive_commands.h"

union can_wavesculptor20 {
    union can_wavesculptor20_rx rx;
    union can_wavesculptor20_tx tx;
};

#endif
