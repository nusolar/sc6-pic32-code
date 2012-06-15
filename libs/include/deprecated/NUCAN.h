/**
 * @file   NUCAN.h
 * @author Chris Yungmann (christopheryungmann2010@u.northwestern.edu)
 * @date   December, 2011
 * @brief  Generic CAN functionality for the pic32mx795f512l.
 */

#ifndef __NUCAN_H
#define __NUCAN_H

#include <plib.h>
#include <stdio.h>
#include <string.h>
#include "../include/NU32v2.h"

#ifndef CAN_BUS_SPEED
    #define CAN_BUS_SPEED                   1000000 // in bits/sec
#endif

#ifndef PROPAGATION_SEG_TQ
    #define PROPAGATION_SEG_TQ              CAN_BIT_1TQ
#endif

#ifndef PHASE_SEG_1_TQ
    #define PHASE_SEG_1_TQ                  CAN_BIT_5TQ
#endif

#ifndef PHASE_SEG_2_TQ
    #define PHASE_SEG_2_TQ                  CAN_BIT_3TQ
#endif

#ifndef PHASE_SEG_2_TIME_SELECT
    #define PHASE_SEG_2_TIME_SELECT         TRUE
#endif

#ifndef SAMPLE_3_TIME
    #define SAMPLE_3_TIME                   TRUE
#endif

#ifndef SYNC_JUMP_WIDTH
    #define SYNC_JUMP_WIDTH                 CAN_BIT_1TQ
#endif

/**
 * Initializes the given CAN module for both transmit and receive, with or without receive interrupt support. Uses standard ids and no filtering.
 * @param[in]   module              The CAN_MODULE to initialize. CAN_MODULE is an enum defined in [c32 compiler folder]\\pic32-libs\include\peripheral\CAN.h, and can be either CAN1 or CAN2.
 * @param[in]   enableInterrupts    Sets up the specified CAN module for CAN receive interrupts.
 */
void
NUCANInitializeModule(CAN_MODULE module, BOOL enableInterrupts);

/**
 * Transmits an array of bytes over the CAN bus using the specified module.
 * @param[in]   module  The CAN_MODULE to use when sending the data. CAN_MODULE is an enum defined in [c32 compiler folder]\\pic32-libs\include\peripheral\CAN.h, and can be either CAN1 or CAN2.
 * @param[in]   id      The 11-bit identifier to assign the data packet. Can be from 0x00 to 0x7F.
 * @param[in]   len     The length of the @a data BYTE array.
 */
int
NUCANTx (CAN_MODULE module, unsigned int id, const BYTE *data, size_t len);

/**
 *  Transmits a null-terminated string over the CAN bus using the specified module.
 */
int
NUCANTxStr(CAN_MODULE module, unsigned int id, const char *str);

int
NUCANRx (CAN_MODULE module, unsigned int *id, BYTE rxData[8], unsigned int *len);

void
UnpackTwoFloats (const BYTE packed[8], float *float1, float *float2);

void
PackTwoFloats (BYTE unpacked[8], float float1, float float2);

#endif
