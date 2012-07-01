#ifndef __NU_CAN_H
#define __NU_CAN_H

#include <plib.h>
#include "can_nu.h"
#include "error_reporting.h"
#include "errorcodes.h"
#include "nu32.h"
#include "timer.h"

#define error_reporting_dev_to_can(erdp)   \
        container_of((erdp), struct can, erd)

enum IDType {
    STANDARD_ID,
    EXTENDED_ID
};

enum PhaseSeg2TimeSelect {
    MAN_SET,
    AUTO_SET
};

enum SampleTimes {
    ONE_TIME,
    THREE_TIMES
};

struct can {
    const struct vtblCANPort   *op;
    
    struct error_reporting_dev  erd;
    CAN_CHANNEL                 error_reporting_can_chn;
    enum IDType                 error_reporting_can_use_extended_id;
    uint16_t                    error_reporting_can_std_id;
    uint16_t                    error_reporting_can_ext_id;

    CAN_MODULE module;
    /* max of 32 channels, each with a 32-message buffer */
    BYTE memoryBuffer[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];
};

struct vtblCANPort {
    int32_t     (*tx)           (const struct can *self, CAN_CHANNEL channel, enum IDType IDTypeExtended, uint16_t sid,
                                    uint32_t eid, uint32_t rtr, const void *data, size_t len);
    long int    (*rx)           (const struct can *self, CAN_CHANNEL channel, unsigned int *id,
                                    void *dest);
    int         (*addChannelTx) (const struct can *self, CAN_CHANNEL channel,
                                    unsigned int channelMsgSize, CAN_TX_RTR rtrEnabled,
                                    CAN_TXCHANNEL_PRIORITY txPriority, CAN_CHANNEL_EVENT interruptEvents);
    int         (*addChannelRx) (const struct can *self, CAN_CHANNEL channel,
                                    unsigned int channelMsgSize, CAN_RX_DATA_MODE dataOnly,
                                    CAN_CHANNEL_EVENT interruptEvents);
    int32_t     (*addFilter)    (const struct can *self, CAN_CHANNEL chn,
                                    CAN_FILTER filter, CAN_ID_TYPE filterType,
                                    uint32_t id, CAN_FILTER_MASK mask,
                                    CAN_FILTER_MASK_TYPE mide, uint32_t maskBits);
};

int32_t
can_new(struct can *self, CAN_MODULE module,
        uint32_t canBusSpeedHz, CAN_BIT_TQ phaseSeg2Tq,
        CAN_BIT_TQ phaseSeg1Tq, CAN_BIT_TQ propSegTq,
        enum PhaseSeg2TimeSelect selectAutoSet,
        enum SampleTimes sample3Times, CAN_BIT_TQ syncJumpWidth,
        CAN_MODULE_EVENT interruptEvents, INT_PRIORITY intPriority,
        CAN_MODULE_FEATURES moduleFeatures);

int32_t
can_new_easy (struct can *self, CAN_MODULE module,
                CAN_MODULE_EVENT interruptEvents,
                INT_PRIORITY intPriority);

#endif
