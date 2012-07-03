#ifndef NU_CAN_ALL_H
#define NU_CAN_ALL_H

#ifdef __PIC32MX
#include <GenericTypedefs.h>
#else
typedef unsigned char BYTE;
#endif

#include <stdint.h>

#include "can_addresses.h"
#include "can_bms.h"
#include "can_common.h"
#include "can_dc.h"
#include "can_mppt.h"
#include "can_ws20.h"

union can_anyFrame {
    union   can_bms     bms;
    union   can_common  common;
    union   can_dc      dc;
    union   can_mppt    mppt;
    union   can_ws20    ws20;    
};

STATIC_ASSERT(sizeof(union can_anyFrame) <= 8, SIZE_MISMATCH);

#endif
