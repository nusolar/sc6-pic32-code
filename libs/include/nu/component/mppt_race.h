#ifndef NU_MPPT_RACE_H
#define NU_MPPT_RACE_H 1

#include "nu/compiler.h"
#include <stdint.h>
#ifdef __PIC32MX
#include <GenericTypeDefs.h>
#endif

const uint16_t MASTER_REQUEST_FRAME_BASE_ID = 0b1110001<<4;
const uint16_t MPPT_ANSWER_FRAME_BASE_ID = 0b1110111<<4;

#define MPPT_MASTER_REQUEST_FRAME_SID(mpptIdOffset) \
            (MASTER_REQUEST_FRAME_BASE_ID + ((mpptIdOffset)<<1) + 1)
#define MPPT_ANSWER_FRAME_SID(mpptIdOffset) \
            (MPPT_ANSWER_FRAME_BASE_ID + ((mpptIdOffset)<<1) + 1)

union PACKED answerDataField {
    struct PACKED {
        /** battery voltage level reached */
        unsigned bvlr       :1;
        /** overtemperature */
        unsigned ovt        :1;
        /** no charge */
        unsigned noc        :1;
        /** undervoltage */
        unsigned undv       :1;
        unsigned reserved   :2;
        /** voltage in (array voltage) */
        unsigned uIn        :10;
        unsigned reserved2  :6;
        /** current in (array current) */
        unsigned iIn        :10;
        unsigned reserved3  :6;
        /** voltage out */
        unsigned uOut       :10;
        /** ambient temperature in degrees C (steps of 5 degrees ) */
        BYTE     tAmbDegreesC;
    };
    BYTE dataBytes[7];
};

#endif
