/*
 * Note: Do not include this file in anything other than main.c
 */

#ifndef __NU_BMS_MAIN_H
#define __NU_BMS_MAIN_H

#ifdef __PIC32MX
#include <GenericTypeDefs.h>
#else
typedef unsigned char BYTE;
#endif

#include <stdint.h>

#include "ad7685.h"
#include "can.h"
#include "can_nu.h"
#include "ds18x20.h"
#include "error_reporting.h"
#include "flash.h"
#include "hais50p.h"
#include "ltc6803.h"
#include "led.h"
#include "nokia5110.h"
#include "nu32.h"
#include "serial.h"
#include "utility.h"

/* config pragmas needed mostly for PICKit3: */
/* ICE/ICD Comm Channel Select: ICE EMUC2/EMUD2 pins shared with PGC2/PGD2 */
#pragma config ICESEL   = ICS_PGx2
/* PLL Multiplier: 20x Multiplier */
#pragma config FPLLMUL  = MUL_20
/* PLL Input Divider: 2x Divider*/
#pragma config FPLLIDIV = DIV_2
/* System PLL Output Clock Divider: PLL Divide by 1 */
#pragma config FPLLODIV = DIV_1
/* Periphearal [Bus] Clock Divisor: Pb_Clk = Sys_Clk/1 */
#pragma config FPBDIV   = DIV_1
/* Primary Oscillator Configuration Mode: XT osc mode */
#pragma config POSCMOD  = XT
/* Oscillator Selection Bits: Primary Oscillator w/ PLL */
#pragma config FNOSC = PRIPLL

/* disable WDT (we'll control it in software) */
#pragma config FWDTEN   = OFF

/* 2.048s WDT timeout (1:2048 postscaler) */
#pragma config WDTPS    = PS2048

struct flashData {
    struct {
        int32_t         module;
        enum tripCode   code;
    } tripInfo;
    uint32_t    currentCount;
};

const uint32_t      SYS_CLK_HZ              = 80000000; /* 80 MHz */
const uint32_t      LTC6803_COUNT           = 3;
const uint32_t      MODULE_COUNT            = 32;
const float         OVER_VOLTAGE            = 4.3;
const float         UNDER_VOLTAGE           = 3.0;
/** Open-wire checking interval in seconds */
const float         OPEN_WIRE_INTERVAL_S    = 2;
const uint32_t      AD7685_COUNT            = 2;
/** Position of battery current sensor ADC in daisy-chain */
const uint32_t      I_SENSOR_BATT           = 0;
/** Position of array current sensor ADC in daisy-chain */
const uint32_t      I_SENSOR_ARRAY          = 1;
/* Discharging: negative current */
const float         OVER_CURRENT_DISCHARGE_A= -60;
/* Charging: positive current */
const float         OVER_CURRENT_CHARGE_A   = 30;
const float         TEMP_READ_INTERVAL_S    = 1;
const float         OVER_TEMP_C             = 50;
const float         UNDER_TEMP_C            = -15;

const IoPortId      LED_ERR_PINLTR          = IOPORT_A;
const uint32_t      LED_ERR_PINNUM          = 0;

const UART_MODULE   SERIAL_MODULE           = NU32_UART_MODULE;
const uint32_t      SERIAL_BAUD             = 9600;

const SpiChannel    NOKIA_SPI_CHANNEL       = SPI_CHANNEL3;
const IoPortId      NOKIA_CS_PIN_LTR        = IOPORT_E;
const uint32_t      NOKIA_CS_PIN_NUM        = BIT_2;
const IoPortId      NOKIA_RESET_PIN_LTR     = IOPORT_E;
const uint32_t      NOKIA_RESET_PIN_NUM     = BIT_1;
const IoPortId      NOKIA_DC_PIN_LTR        = IOPORT_E;
const uint32_t      NOKIA_DC_PIN_NUM        = BIT_0;

const SpiChannel    LTC6803_SPI_CHN         = SPI_CHANNEL4;
const IoPortId      LTC6803_CS_PIN_LTR      = IOPORT_G;
const uint32_t      LTC6803_CS_PIN_NUM      = BIT_9;

const CAN_MODULE    COMMON_CAN_MOD          = CAN2;
const CAN_CHANNEL   COMMON_CAN_TX_CHN       = CAN_CHANNEL0;
const CAN_CHANNEL   COMMON_CAN_RX_CHN       = CAN_CHANNEL1;

const SpiChannel    ADC_SPI_CHN             = SPI_CHANNEL2;
const IoPortId      ADC_CS_PIN_LTR          = IOPORT_A;
const uint32_t      ADC_CS_PIN_NUM          = BIT_0;

const IoPortId      DS18X20_PIN_LTR         = IOPORT_E;
const uint32_t      DS18X20_PIN_NUM         = BIT_0;

/** Lookup table of the ROM codes for all the DS18X20 temp sensors */
const union romCode DS18X20_ROMCODES[] = {
    /* @TODO fill in romcodes */
#warning "DS18X20 romcodes need to be filled in"
    /* bar 1 */
    [0]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [1]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [2]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [3]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [4]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    /* bar 2 */
    [5]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [6]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [7]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [8]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [9]     = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    [10]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [11]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [12]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [13]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [14]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    [15]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [16]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [17]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [18]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [19]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    [20]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [21]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [22]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [23]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [24]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    [25]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [26]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [27]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [28]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [29]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [30]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    [31]    = {.byteArr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
};

/* there must be at least 32 DS18X20 romcodes to correspond with at least 32
 *      temp sensors (per regulations)
 */
STATIC_ASSERT(ARRAY_SIZE(DS18X20_ROMCODES) >= 32, NOT_ENOUGH_DS18X20_ROMCODES);

/** Cause of the last PIC reset to be reported right after the car comes
 *  back up.
 */
enum lastReset {
    POWER_ON_RESET = 1,
    BROWNOUT_RESET,
    LOW_VOLTAGE_RESET,
    MASTER_CLEAR_RESET,
    WDT_RESET,
    SOFTWARE_RESET,
    CONFIG_MISMATCH_RESET,
};

/* Function Prototypes */

enum lastReset
getLastResetCause(void);

/** Initialize all the devices and register error-reporting devices */
int32_t
init_devices(struct nokia5110 *dp, struct ltc6803 *ltcp,
        struct ad7685 *adp,
        struct ds18x20 *dsp, struct can *canp,
        struct serial *serp,
        struct led *led0p, struct led *led1p) __attribute__((nonnull));

int32_t
sendLastResetCode(struct can *self) __attribute__((nonnull));

/** Trip the car, reporting and saving the tripcode as well as the relevant
 *  module (or -1 if no relevant module)
 */
void
nu_trip(struct can *canp, enum tripCode code, uint32_t module)
__attribute__((noreturn, nonnull));

#define TRIP_NOMOD(tripCode)                                            \
    do {                                                                \
        REPORT((REP_EMERGENCY, "tripping with code %d (%s)", tripCode,  \
            tripcodeStr[tripCode]));                                    \
        nu_trip(canp, (tripCode), 0xFFFFFFFF);                          \
    } while(0)
  
#define TRIP_MOD(tripCode, module)                                      \
    do {                                                                \
        REPORT((REP_EMERGENCY, "tripping with code %d (%s), module %d", \
                tripCode, tripcodeStr[tripCode], module));              \
        nu_trip(canp, (tripCode), (module));                            \
    } while(0)

#endif
