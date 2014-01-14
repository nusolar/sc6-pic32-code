#include "nu/peripheral/can/tx.h"
#include "nu/peripheral/can/setup.h"
#include "nu/peripheral/can.h"
#include "nu/errorcodes.h"
#include "nu/utility/bits.h"
#include <peripheral/CAN.h>

struct nu_can_tx_attr {
    u32 eid;
    CAN_CHANNEL chn;
    enum nu_can_id_type id_type;
    u16 sid;
    bool rtr;
    u8 padding;
};

s32
nu_can_tx(const struct nu_can *c, const struct nu_can_tx_attr *a,
          const void *src, size_t n) {
    CANTxMessageBuffer *message = CANGetTxMessageBuffer(c->module, a->chn);
    if (n > 8)
        return -EINVAL;

    CANSetOperatingMode(c->module, CAN_NORMAL_OPERATION);
    while (CAN_NORMAL_OPERATION != CANGetOperatingMode(c->module))
        Nop();

    /* clear message data */
    memset(message, 0, sizeof(*message));

    /* insert SID/EID information */
    message->msgEID.IDE = (NU_CAN_EXTENDED_ID == a->id_type);
    message->msgSID.SID = BITFIELD_CAST(a->sid, 11);  /* 11   bits */
    message->msgEID.DLC = BITFIELD_CAST(n, 4);   /* 4    bits */
    message->msgEID.RTR = BITFIELD_CAST(a->rtr, 1);   /* 1    bit; 1 = remote transmission request enabled */
    if (NU_CAN_EXTENDED_ID == a->id_type) /* EID is indicated by IDTypeExtended = 1 */
        message->msgEID.EID = BITFIELD_CAST(a->eid, 18);    /* 18 bits */

    if (n)
        memcpy(message->data, (const byte *)src, n);

    CANUpdateChannel(c->module, a->chn);
    CANFlushTxChannel(c->module, a->chn);

    return 0;
}
