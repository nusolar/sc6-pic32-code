#include "../include/NUCAN.h"

void
NUCANInitializeModule(CAN_MODULE module, BOOL enableInterrupts)
{
    CANEnableModule(module, TRUE);
    
    CANSetOperatingMode(module, CAN_CONFIGURATION);
    while(CANGetOperatingMode(module) != CAN_CONFIGURATION)
        ; // do nothing
    
    CAN_BIT_CONFIG canBitConfig;
    canBitConfig.propagationSegTq    = PROPAGATION_SEG_TQ;
    canBitConfig.phaseSeg1Tq         = PHASE_SEG_1_TQ;
    canBitConfig.phaseSeg2Tq         = PHASE_SEG_2_TQ;
    canBitConfig.phaseSeg2TimeSelect = PHASE_SEG_2_TIME_SELECT;
    canBitConfig.sample3Time         = SAMPLE_3_TIME;
    canBitConfig.syncJumpWidth       = SYNC_JUMP_WIDTH;
    
    CANSetSpeed(module, &canBitConfig, SYS_FREQ, CAN_BUS_SPEED);
    
    // Define and assign a CAN 1 memory buffer for 1 TX channels and 1 RX
    // channel, each with 8 message buffers.
    // CAN_TX_RX_MESSAGE_SIZE_BYTES is defined in can.h
    
    BYTE *canFifo = 0;
    if (module == CAN1) {
        static BYTE CAN1MessageFifoArea[2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES];
        canFifo = CAN1MessageFifoArea;
    } else if (module == CAN2) {
        static BYTE CAN2MessageFifoArea[2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES];
        canFifo = CAN2MessageFifoArea;
    }
    
    CANAssignMemoryBuffer(module, canFifo, (2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES)); // THIS FUNCTION DOES NOT ACTUALLY USE THE PROVIDED SIZE!!!!!!!! >:(   :'(
    
    CANConfigureChannelForTx(module, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
    CANConfigureChannelForRx(module, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE);
    
    CANConfigureFilter(module, CAN_FILTER0, 0, CAN_SID);
    CANConfigureFilterMask(module, CAN_FILTER_MASK0, 0, CAN_SID, CAN_FILTER_MASK_ANY_TYPE);
    CANLinkFilterToChannel(module, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
    CANEnableFilter(module, CAN_FILTER0, TRUE);
    
    if (enableInterrupts) {
        /* Step 6: Enable interrupt and events. Enable the receive
         * channel not empty event (channel event) and the receive
         * channel event (CAN1 event).
         * The interrrupt peripheral library is used to enable
         * the CAN interrupt to the CPU. */
    
        CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
        CANEnableModuleEvent (CAN1, CAN_RX_EVENT, TRUE);
    
        /* These functions are from interrupt peripheral
         * library. */
        
        INTSetVectorPriority(INT_CAN_1_VECTOR, INT_PRIORITY_LEVEL_4);
        INTSetVectorSubPriority(INT_CAN_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(INT_CAN1, INT_ENABLED);
    }
    
    CANSetOperatingMode(module, CAN_NORMAL_OPERATION);
    while (CANGetOperatingMode(module) != CAN_NORMAL_OPERATION)
        ; // do nothing
}

int
NUCANTx (CAN_MODULE module, unsigned int id, const BYTE *data, size_t len)
{
    CANTxMessageBuffer *msgBuffer = CANGetTxMessageBuffer(module, CAN_CHANNEL0);
    if(msgBuffer != NULL) {
        // Load the message here
        //
        //    typedef union {
        //        struct {
        //        CAN_TX_MSG_SID msgSID;
        //        CAN_MSG_EID msgEID;
        //        BYTE data[8];
        //    }
        //    UINT32 messageWord[4];
        //} CANTxMessageBuffer;
        
        memset(msgBuffer->messageWord, 0, 4*sizeof(UINT32));
        
        msgBuffer->msgSID.SID       = id;
        msgBuffer->msgEID.IDE       = 0;
        msgBuffer->msgEID.DLC       = len > 8 ? 8 : len;
        memcpy(msgBuffer->data, data, len > 8 ? 8 : len);
        
        CANUpdateChannel(module, CAN_CHANNEL0);
        CANFlushTxChannel(module, CAN_CHANNEL0);
        
        return 0;
    } else {
        // No space in the channel
        return 1;
    }
}

int
NUCANTxStr(CAN_MODULE module, unsigned int id, const char *str)
{
    return NUCANTx(module, id, (const BYTE *)str, strlen(str));
}

int
NUCANRx (CAN_MODULE module, unsigned int *id, BYTE rxData[8], unsigned int *len)
{
    CANRxMessageBuffer *rxMsg = CANGetRxMessage(module, CAN_CHANNEL1);
    
    if (rxMsg == NULL)
        return 1;
    
    *id = (int)(rxMsg->msgSID.SID);
    
    *len = (int)(rxMsg->msgEID.DLC);
    
    if (*len == 0)
        return 2;
    
    memcpy(rxData, rxMsg->data, *len);
    
    CANUpdateChannel(module, CAN_CHANNEL1);
    
    return 0; 
} 

void
UnpackTwoFloats (const BYTE packed[8], float *float1, float *float2)
{
    memcpy(float1, packed, 4);
    memcpy(float2, packed+4, 4);
}

void
PackTwoFloats (BYTE unpacked[8], float float1, float float2)
{
    memcpy(unpacked, &float1, 4);
    memcpy(unpacked+4, &float2, 4);
}
