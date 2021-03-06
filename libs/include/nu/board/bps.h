#ifndef NU_BPS_H
#define NU_BPS_H

#include "nu/peripheral/pinctl.h"
#include "nu/types.h"

/** Trip codes that will be reported right before the car trips, and [hopefully]
 *  right after the car comes back up.
 */
#define BMS_TRIPCODES(BMS_TRIPCODE) \
    BMS_TRIPCODE(NONE) \
    BMS_TRIPCODE(OTHER) \
    BMS_TRIPCODE(OW_BUS_FAILURE) \
    BMS_TRIPCODE(DS18X20_MISSING) \
    BMS_TRIPCODE(LTC_POST_FAILED) \
    BMS_TRIPCODE(ADC_FAILURE) \
    BMS_TRIPCODE(OVER_VOLTAGE) \
    BMS_TRIPCODE(UNDER_VOLTAGE) \
    BMS_TRIPCODE(OVER_CURRENT_DISCHRG) \
    BMS_TRIPCODE(OVER_CURRENT_CHRG) \
    BMS_TRIPCODE(OVER_TEMP) \
    BMS_TRIPCODE(UNDER_TEMP)

#define BMS_TRIPCODE_ENUM(x) BMS_TRIP_##x,
#define BMS_TRIPCODE_STR(x) #x,

enum nu__BPS__Tripcode {
    BMS_TRIPCODES(BMS_TRIPCODE_ENUM)
};
extern char const * const nu__BPS__tripcode_str[];

//static const NU_PIN(pin_main_relay,     NU_PIN_D2);
//static const NU_PIN(pin_array_relay,    NU_PIN_D3);
//static const struct nu__Pin pin_batt_bypass = NU_PIN_INIT(IOPORT_D, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5);

s32
nu__BPS__main(void);

#if 0

#define HZ (80000000UL)

#include "ad7685.h"
#include "can.h"
#include "common_pragmas.h"
#include "compiler.h"
#include "ds18x20.h"
#include "error_reporting.h"
#include "flash.h"
#include "hais50p.h"
#include "ltc6803.h"
#include "nokia5110.h"
#include "nu_types.h"
#include "nu32.h"
#include "pinctl.h"
#include "utility.h"
#include "wdt.h"

/** Trip codes that will be reported right before the car trips, and [hopefully]
 *  right after the car comes back up.
 */
#define TRIPCODES                   \
    TRIPCODE(NONE)                  \
    TRIPCODE(OTHER)                 \
    TRIPCODE(OW_BUS_FAILURE)        \
    TRIPCODE(DS18X20_MISSING)       \
    TRIPCODE(LTC_POST_FAILED)       \
    TRIPCODE(ADC_FAILURE)           \
    TRIPCODE(OVER_VOLTAGE)          \
    TRIPCODE(UNDER_VOLTAGE)         \
    TRIPCODE(OVER_CURRENT_DISCHRG)  \
    TRIPCODE(OVER_CURRENT_CHRG)     \
    TRIPCODE(OVER_TEMP)             \
    TRIPCODE(UNDER_TEMP)

enum tripcode {
#define TRIPCODE(x) TRIP_##x,
    TRIPCODES
#undef TRIPCODE
};

static const char * const tripcode_s[] = {
#define TRIPCODE(x) #x,
    TRIPCODES
#undef TRIPCODE
};

struct flashData {
    s32             last_trip_module;
    enum tripcode   last_tripcode;
    char            last_trip_file[20];
    u32             last_trip_line;
    float           last_trip_temp;
    double          cc_battery;
    double          cc_array;
    double          cc_mppt1;
    double          cc_mppt2;
    double          cc_mppt3;
    double          wh_mppt1in;
    double          wh_mppt1out;
    double          wh_mppt2in;
    double          wh_mppt2out;
    double          wh_mppt3in;
    double          wh_mppt3out;
    double          wh_battery;
};

/** Cause of the last PIC reset to be reported right after the car comes
 *  back up.
 */
#define RESET_CAUSES                \
    RESET_CAUSE(OTHER)              \
    RESET_CAUSE(POWER_ON)           \
    RESET_CAUSE(BROWNOUT)           \
    RESET_CAUSE(LOW_VOLTAGE)        \
    RESET_CAUSE(MASTER_CLEAR)       \
    RESET_CAUSE(WDT)                \
    RESET_CAUSE(SOFTWARE)           \
    RESET_CAUSE(CONFIG_MISMATCH)

enum lastReset {
#define RESET_CAUSE(x)  x##_RESET,
    RESET_CAUSES
#undef RESET_CAUSE
};

static const char *lastResetStr[] = {
#define RESET_CAUSE(x)  #x "_RESET",
    RESET_CAUSES
#undef RESET_CAUSE
};

/**************************
 * configuration settings
 **************************/

static const uint32_t bms_sys_clk_hz = 80000000; /* 80 MHz */

/******
 * limits
 */

static const float  over_voltage                    = 4.3;
static const float  under_voltage                   = 2.75;
static const float  under_current_array_a           = -1;
static const float  over_current_array_a            = 10;

/* Discharging: negative current */
static const float  over_current_batt_discharge_a   = 72.8;
/* Charging: positive current */
static const float  over_current_batt_charge_a      = -36.4;
/* actually 45, but the reduction is to pass scrutineering */
static const float  over_temp_c                     = 35;
static const float  under_temp_c                    = 0;

/********
 * time intervals
 */

static const float interval_get_voltages        = 0;
/* open wire takes priority over normal voltage conversion */
static const float interval_get_ow_voltages     = 5;
static const float interval_get_temps           = 0;
static const float interval_get_currents        = 0;
static const float interval_tx_heartbeat        = 1;
static const float interval_tx_uptime           = 5;
static const float interval_tx_lastreset        = 5;
static const float interval_tx_lasttrip         = 5;
static const float interval_tx_battbypass       = 1;
static const float interval_tx_currents         = 1;
static const float interval_tx_voltages         = 1;
static const float interval_tx_ow_voltages      = 5;
static const float interval_tx_temps            = 1;
static const float interval_tx_cc_array         = 1;
static const float interval_tx_cc_battery       = 1;
static const float interval_tx_wh_batt          = 1;
static const float interval_tx_trip_pt_current  = 5;
static const float interval_tx_trip_pt_voltage  = 5;
static const float interval_tx_trip_pt_temp     = 5;
static const float interval_tx_mpptrequests     = 1;
static const float interval_serial_tx           = 0;
static const float interval_save_flash          = 100;

/*********
 * pins
 */

#define BMS_PINS                    \
    BMS_PIN(nokia_dc,       E, 9)   \
    BMS_PIN(nokia1_cs,      G, 9)   \
    BMS_PIN(nokia1_reset,   A, 9)   \
    BMS_PIN(nokia2_cs,      E, 8)   \
    BMS_PIN(nokia2_reset,   A, 10)  \
    BMS_PIN(ltc6803_cs,     D, 9)   \
    BMS_PIN(ds18b20,        A, 0)   \
    BMS_PIN(main_relay,     D, 2)   \
    BMS_PIN(array_relay,    D, 3)
static const struct pin pin_batt_bypass = PIN_INIT(IOPORT_B, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5);
#define BMS_PIN(name,ltr,num)   \
static const struct pin pin_##name = PIN_INIT(IOPORT_##ltr,BIT_##num);
BMS_PINS
#undef BMS_PIN

/* SPI */
static const SpiChannel    nokia_spi_channel       = SPI_CHANNEL2;
static const SpiChannel    ltc6803_spi_chn         = SPI_CHANNEL1;

/* CAN */
static const CAN_MODULE    common_can_mod          = CAN1;
static const CAN_MODULE    mppt_can_mod            = CAN2;

/*******
 * peripheral config
 */

static const CAN_CHANNEL   common_can_rx_chn       = CAN_CHANNEL0;
static const CAN_CHANNEL   common_can_tx_chn       = CAN_CHANNEL1;
static const CAN_CHANNEL   common_can_tx_err_chn   = CAN_CHANNEL2;
static const CAN_CHANNEL   mppt_can_rx_chn         = CAN_CHANNEL0;
static const CAN_CHANNEL   mppt_can_tx_chn         = CAN_CHANNEL1;

#define                    LTC6803_COUNT            3
static const uint32_t      module_count            = 32;
#define                    AD7685_COUNT             2
/** Position of battery current sensor ADC in daisy-chain */
static const uint32_t      i_sensor_batt           = 0;
/** Position of array current sensor ADC in daisy-chain */
static const uint32_t      i_sensor_array          = 1;

static const uint32_t      serial_baud             = 9600;

/** Lookup table of the ROM codes for all the DS18X20 temp sensors */
static const union romcode ds18b20_romcodes[] = {
    /* bar 1 */
    [0]     = {.bytes = {0x28, 0x9E, 0x63, 0xEA, 0x02, 0x00, 0x00}},
    [1]     = {.bytes = {0x28, 0x63, 0x58, 0xEA, 0x02, 0x00, 0x00}},
    [2]     = {.bytes = {0x28, 0xB4, 0x6B, 0xEA, 0x02, 0x00, 0x00}},
    [3]     = {.bytes = {0x28, 0x77, 0x55, 0xEA, 0x02, 0x00, 0x00}},
    [4]     = {.bytes = {0x28, 0x73, 0x58, 0xEA, 0x02, 0x00, 0x00}},

    /* bar 2 */
    [5]     = {.bytes = {0x28, 0xF3, 0x5F, 0xEA, 0x02, 0x00, 0x00}},
    [6]     = {.bytes = {0x28, 0xCF, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
    [7]     = {.bytes = {0x28, 0xAF, 0x67, 0xEA, 0x02, 0x00, 0x00}},
    [8]     = {.bytes = {0x28, 0x50, 0x74, 0xEA, 0x02, 0x00, 0x00}},
    [9]     = {.bytes = {0x28, 0xB9, 0x56, 0xEA, 0x02, 0x00, 0x00}},

    [10]    = {.bytes = {0x28, 0x27, 0x61, 0xEA, 0x02, 0x00, 0x00}},
    [11]    = {.bytes = {0x28, 0x0E, 0x6E, 0xEA, 0x02, 0x00, 0x00}},
    [12]    = {.bytes = {0x28, 0xE9, 0x2F, 0xEA, 0x02, 0x00, 0x00}},
    [13]    = {.bytes = {0x28, 0x2E, 0x5B, 0xEA, 0x02, 0x00, 0x00}},
    [14]    = {.bytes = {0x28, 0x82, 0x5B, 0xEA, 0x02, 0x00, 0x00}},

    [15]    = {.bytes = {0x28, 0xED, 0x65, 0xEA, 0x02, 0x00, 0x00}},
    [16]    = {.bytes = {0x28, 0x77, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
    [17]    = {.bytes = {0x28, 0xC3, 0x5A, 0xEA, 0x02, 0x00, 0x00}},
    [18]    = {.bytes = {0x28, 0x9D, 0x6D, 0xEA, 0x02, 0x00, 0x00}},
    [19]    = {.bytes = {0x28, 0x1C, 0x3E, 0xEA, 0x02, 0x00, 0x00}},

    [20]    = {.bytes = {0x28, 0x4E, 0x66, 0xEA, 0x02, 0x00, 0x00}},
    [21]    = {.bytes = {0x28, 0x76, 0x6D, 0xEA, 0x02, 0x00, 0x00}},
    [22]    = {.bytes = {0x28, 0xF6, 0x73, 0xEA, 0x02, 0x00, 0x00}},
    [23]    = {.bytes = {0x28, 0xDE, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
    [24]    = {.bytes = {0x28, 0x5C, 0x6B, 0xEA, 0x02, 0x00, 0x00}},

    [25]    = {.bytes = {0x28, 0x79, 0x6A, 0xEA, 0x02, 0x00, 0x00}},
    [26]    = {.bytes = {0x28, 0xD0, 0x63, 0xEA, 0x02, 0x00, 0x00}},
    [27]    = {.bytes = {0x28, 0xE2, 0x62, 0xEA, 0x02, 0x00, 0x00}},
    [28]    = {.bytes = {0x28, 0xF7, 0x70, 0xEA, 0x02, 0x00, 0x00}},
    [29]    = {.bytes = {0x28, 0x0E, 0x6C, 0xEA, 0x02, 0x00, 0x00}},

    [30]    = {.bytes = {0x28, 0x31, 0x62, 0xEA, 0x02, 0x00, 0x00}},
    [31]    = {.bytes = {0x28, 0xD0, 0x6B, 0xEA, 0x02, 0x00, 0x00}},

    /*  S
    []      = {.bytes = {0x28, 0x54, 0x3F, 0xEA, 0x02, 0x00, 0x00}},
    []      = {.bytes = {0x28, 0xD2, 0x60, 0xEA, 0x02, 0x00, 0x00}},
    []      = {.bytes = {0x28, 0x56, 0x70, 0xEA, 0x02, 0x00, 0x00}},
    []      = {.bytes = {0x28, 0x61, 0x65, 0xEA, 0x02, 0x00, 0x00}},
    []      = {.bytes = {0x28, 0x5D, 0x77, 0xEA, 0x02, 0x00, 0x00}},
     */
};

/* there must be at least 32 DS18X20 romcodes to correspond with at least 32
 *      temp sensors (per regulations)
 */
STATIC_ASSERT(ARRAY_SIZE(ds18b20_romcodes) >= 32, NOT_ENOUGH_DS18X20_ROMCODES);

/* in case we want info on the stack... */
extern const unsigned _stack;
extern const unsigned _min_stack_size;

const unsigned stack = (unsigned int)(&_stack);
const unsigned min_stack_size = (unsigned int)(&_min_stack_size);

/*****************
 * device driver declarations
 */
static struct nokia5110 display1;
static struct nokia5110 display2;
static struct ad7685 adc = AD7685_INIT(SPI_CHANNEL4, IOPORT_F, BIT_12, 2, CHAIN_MODE, NO_BUSY_INDICATOR);
static struct ltc6803 ltc;
static struct ds18x20 ds;
static struct can mppt_can;
static struct can common_can;

/**************
 * car state
 */

#define UNINIT 12345
static double   uptime              = 0;
static int32_t  last_trip_module     = UNINIT;
static enum tripcode last_tripcode  = 0;
static char     lastTrip_file[20]   = {0};
static uint32_t last_trip_line       = 0;
static float    last_trip_temp       = 0;
static enum lastReset last_reset_cause = 0;
static uint32_t battBypass          = 0;
static float    currentBattery      = UNINIT;
static float    currentArray        = UNINIT;
static double   cc_battery          = 0;
static double   cc_array            = 0;
static double   cc_mppt1            = 0;
static double   cc_mppt2            = 0;
static double   cc_mppt3            = 0;
static double   wh_battery          = 0;
static double   wh_mppt1in          = 0;
static double   wh_mppt1out         = 0;
static double   wh_mppt2in          = 0;
static double   wh_mppt2out         = 0;
static double   wh_mppt3in          = 0;
static double   wh_mppt3out         = 0;
static float    voltages[12*LTC6803_COUNT] =
    {[0 ... ARRAY_SIZE(voltages)-1] = UNINIT};
static float openWireVoltages[12*LTC6803_COUNT] =
    {[0 ... ARRAY_SIZE(openWireVoltages)-1] = UNINIT};
static float rawCurrents[AD7685_COUNT]          =
    {[0 ... ARRAY_SIZE(rawCurrents)-1]      = UNINIT};
static float temperatures[ARRAY_SIZE(ds18b20_romcodes)] =
    {[0 ... ARRAY_SIZE(temperatures)-1]     = UNINIT};

static ALWAYSINLINE int32_t
commonCanTx(uint16_t sid, void *data, size_t len)
{
    return commonCanp->op->tx(commonCanp, common_can_tx_chn, STANDARD_ID,
            sid, 0, 0, data, len);
}

/* shorthand */
#define ccTx(sid, var) commonCanTx(sid, &var, sizeof(var))

static void
saveFlashNow(void)
{
    struct flashData fd = {
        .cc_array           = cc_array,
        .cc_battery         = cc_battery,
        .cc_mppt1           = cc_mppt1,
        .cc_mppt2           = cc_mppt2,
        .cc_mppt3           = cc_mppt3,
        .last_tripcode      = last_tripcode,
        .last_trip_module    = last_trip_module,
        .last_trip_line      = last_trip_line,
        .last_trip_temp      = last_trip_temp,
        .wh_battery         = wh_battery,
        .wh_mppt1in         = wh_mppt1in,
        .wh_mppt1out        = wh_mppt1out,
        .wh_mppt2in         = wh_mppt2in,
        .wh_mppt2out        = wh_mppt2out,
        .wh_mppt3in         = wh_mppt3in,
        .wh_mppt3out        = wh_mppt3out,
    };

    strncpy(fd.last_trip_file, lastTrip_file, ARRAY_SIZE(fd.last_trip_file)-1);

    clear_wdt();

    writeFlash(&fd, sizeof(fd));
}

/** Trip the car, reporting and saving the tripcode as well as the relevant
 *  module (or -1 if no relevant module)
 */
#ifdef __DEBUG
#warning "DEBUG BUILD: TRIP IGNORED!"
static ALWAYSINLINE void
#else
static ALWAYSINLINE void NORETURN
#endif
nu_trip(const char *file, uint32_t line, enum tripcode code, uint32_t module)
{
    struct can_bms_tx_trip trip = {
        .module     = module,
        .tripCode   = code,
    };

    clear_wdt();
    /* disable use of CLEARWDT() function, such as in saveFlashNow(),
     * as to prevent hanging on trip
     */
    disableClearWdt();

    commonCanTx(ADDR_BMS_TX_TRIP, &trip, sizeof(trip));

    last_tripcode       = code;
    last_trip_module    = module;
    strncpy(lastTrip_file, file, ARRAY_SIZE(lastTrip_file)-1);
    last_trip_line      = line;
    last_trip_temp      = UNINIT;
    if (code == TRIP_UNDER_TEMP || code == TRIP_OVER_TEMP)
        last_trip_temp = temperatures[module];
    saveFlashNow();

#ifdef __DEBUG
    REPORT(REP_WARNING, "DEBUG BUILD: IGNORED TRIP %d (%s) on module %d",
        code, tripcode_s[code], module);
    return;
#else

    PIN_CLEAR(pin_main_relay);

    DisableWDT();

    while (1)
        ; /* do nothing */
#endif
}

#define trip_nomod(tripCode)                                                \
    do {                                                                    \
        REPORT(REP_EMERGENCY, "tripping with code %d (%s)",                 \
            tripCode, tripcode_s[tripCode]);                                \
        nu_trip(__FILE__, __LINE__, tripCode, 0xFFFFFFFF);                  \
    } while(0)

#define trip_mod(tripCode, module)                                  \
    do {                                                            \
        REPORT(REP_EMERGENCY,                                       \
            "tripping with code %d (%s), module %d",                \
            tripCode, tripcode_s[tripCode], module);                \
        nu_trip(__FILE__, __LINE__, tripCode, module);              \
    } while(0)

static void
get_last_reset_cause(void)
{
    clear_wdt();

    if (isPOR()) {              /*
                                 * note that a power-on reset sets BOTH the
                                 * POR and BOR bits (per the pic32 reference
                                 * manual), so the power-on reset condition
                                 * must be checked BEFORE the brown-out reset
                                 * (BOR) condition
                                 */
        mClearPORFlag();
        mClearBORFlag();
        REPORT(REP_INFO, "POR reset");
        last_reset_cause = POWER_ON_RESET;
    } else if (isBOR()) {
        mClearBORFlag();
        REPORT(REP_WARNING, "BOR reset");
        last_reset_cause = BROWNOUT_RESET;
    } else if (isLVD()) {       /* low voltage detect reset
                                 * It seems that the PIC32 libraries don't
                                 * actually support this, as reset.h has this
                                 * definition:
                                 * #define isLVD()							0
                                 */
        REPORT(REP_WARNING, "LVD reset");
        last_reset_cause = LOW_VOLTAGE_RESET;
    } else if (isMCLR()) {      /* master clear (reset) pin reset */
        mClearMCLRFlag();
        REPORT(REP_INFO, "MCLR reset");
        last_reset_cause = MASTER_CLEAR_RESET;
    } else if (isWDTTO()) {     /* watchdog timer reset */
        mClearWDTOFlag();
        REPORT(REP_ERROR, "WDT reset");
        last_reset_cause = WDT_RESET;
    } else if (mGetSWRFlag()) { /* software reset */
        mClearSWRFlag();
        REPORT(REP_WARNING, "SWR reset");
        last_reset_cause = SOFTWARE_RESET;
    } else if (mGetCMRFlag()) { /* config mismatch reset */
        mClearCMRFlag();
        REPORT(REP_CRITICAL, "CMR reset");
        last_reset_cause = CONFIG_MISMATCH_RESET;
    };
}

static ALWAYSINLINE void
init_relays(void)
{
    clear_wdt();

    PIN_SET_DIGITAL_OUT(pin_main_relay);
    PIN_SET_DIGITAL_OUT(pin_array_relay);
    PIN_SET(pin_main_relay);
    PIN_SET(pin_array_relay);
}

static ALWAYSINLINE int32_t
init_leds(void)
{
    int32_t err;

    clear_wdt();

    REPORT_ON_ERR(err = nu32_init_leds(), REP_WARNING, "nu32_init_leds");
    if (nu32_led0p)
        register_reporting_dev(&(nu32_led0p->erd), REP_WARNING);
    if (nu32_led1p)
        register_reporting_dev(&(nu32_led1p->erd), REP_ERROR);

    return err;
}

static ALWAYSINLINE int32_t
init_serial(void)
{
    int32_t err;

    clear_wdt();

    IF_NOERR(err = nu32_init_serial(serial_baud), REP_WARNING,
            "nu32_init_serial error")
        register_reporting_dev(&(nu32_serp->erd), REP_DEBUG);
    else
        return err;

    return 0;
}

static ALWAYSINLINE int32_t
init_nokias(void)
{
    clear_wdt();

    IF_NOERR(nokia5110_new(dp1, nokia_spi_channel,
                                pin_nokia1_cs, pin_nokia1_reset,
                                pin_nokia_dc),
                REP_WARNING, "nokia5110_new")
        register_reporting_dev(&(dp1->erd), REP_DEBUG);
    REPORT_ON_ERR(nokia5110_new(dp2, nokia_spi_channel,
                                pin_nokia2_cs, pin_nokia2_reset,
                                pin_nokia_dc),
                REP_WARNING, "nokia5110_new");

    return 0;
}

static ALWAYSINLINE int32_t
init_cans(void)
{
    clear_wdt();

    IF_NOERR(can_new_easy(commonCanp, common_can_mod, 0, INT_PRIORITY_DISABLED),
            REP_WARNING, "can_new_easy") {

        commonCanp->error_reporting_can_chn = common_can_tx_err_chn;
        commonCanp->error_reporting_can_use_extended_id = STANDARD_ID;
        commonCanp->error_reporting_can_std_id = ADDR_BMS_TX_ERROR;
        commonCanp->error_reporting_can_ext_id = 0;

        commonCanp->op->addChannelRx(commonCanp, common_can_rx_chn,
            32, CAN_RX_FULL_RECEIVE, 0);
        commonCanp->op->addFilter(commonCanp, common_can_rx_chn, CAN_FILTER0,
            CAN_SID, ADDR_BMS_RX_BASE, CAN_FILTER_MASK0,
            CAN_FILTER_MASK_IDE_TYPE, 0x7F0);
        commonCanp->op->addChannelTx(commonCanp, common_can_tx_chn,
            32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
        commonCanp->op->addChannelTx(commonCanp, common_can_tx_err_chn,
            32, CAN_TX_RTR_DISABLED, CAN_LOWEST_PRIORITY, 0);

        register_reporting_dev(&(commonCanp->erd), REP_DEBUG);
    }

    IF_NOERR(can_new(mpptCanp, mppt_can_mod, 125E3, CAN_BIT_2TQ, CAN_BIT_4TQ,
            CAN_BIT_1TQ, AUTO_SET, THREE_TIMES, CAN_BIT_1TQ, 0,
            INT_PRIORITY_DISABLED, 0),
            REP_WARNING, "can_new_easy") {
        mpptCanp->op->addChannelRx(mpptCanp, mppt_can_rx_chn,
            32, CAN_RX_FULL_RECEIVE, 0);
        mpptCanp->op->addChannelTx(mpptCanp, mppt_can_tx_chn,
            32, CAN_TX_RTR_ENABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
    }

    return 0;
}

static ALWAYSINLINE int32_t
init_ltcs(void)
{
    union BpsConfig cfg[LTC6803_COUNT];

    clear_wdt();

    memset(cfg, 0, sizeof(cfg));

    /* LTC6803 configuration: measurement only mode,
     * 13ms minimum measurement time */
    cfg[0].cfgr0 = WDT | LVLPL | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(over_voltage);
    cfg[0].vuv = convertUVLimit(under_voltage);
    cfg[2] = cfg[1] = cfg[0];

    IF_ERR(ltc6803_new(ltcp, ltc6803_spi_chn, pin_ltc6803_cs.ltr,
                    pin_ltc6803_cs.num, LTC6803_COUNT, cfg),
                REP_CRITICAL, "ltc6803_new")
        trip_nomod(TRIP_OTHER);

    IF_ERR(ltcp->op->post(ltcp), REP_EMERGENCY, "LTC POST FAILED")
        trip_nomod(TRIP_LTC_POST_FAILED);

    return 0;
}

static ALWAYSINLINE int32_t
init_adcs(void)
{
    s32 err;
    u32 tmp[2];

    clear_wdt();

    IF_ERR(err = ad7685_init(&adc),
            REP_CRITICAL, "ad7685_new")
        return err;

    ad7685_convert_read_uv(&adc, tmp);

    return 0;
}

static ALWAYSINLINE int32_t
init_ds18b20s(void)
{
    int32_t tmp;
    uint32_t ui;

    clear_wdt();

    IF_ERR(ds_new(dsp, pin_ds18b20, PARASITIC_POWER_DISABLE),
                REP_CRITICAL, "ds18x20_new")
        trip_nomod(TRIP_OTHER);

    if ((tmp = ds_find_devices(dsp, NULL, 0)) != module_count) {
        REPORT(REP_EMERGENCY, "DS18X20: found %d, expected %d", tmp, module_count);
        trip_nomod(TRIP_DS18X20_MISSING);
    }

    /* DS18b20 Presence Test */
    for (ui = 0; ui < ARRAY_SIZE(ds18b20_romcodes); ui++) {
        clear_wdt();
        IF_ERR(tmp = ds_verify(dsp, ds18b20_romcodes[ui]),
                REP_EMERGENCY, "DS18X20 verify failed on romcode %d", ui) {
            trip_mod(TRIP_DS18X20_MISSING, ui);
        } else if (!tmp) {
            /* Missing Sensor */
            REPORT(REP_EMERGENCY,
                    "MISSING TEMP %d, ROMCODE %02X %02X %02X %02X %02X %02X %02X",
                    ui,
                    ds18b20_romcodes[ui].bytes[0],
                    ds18b20_romcodes[ui].bytes[1],
                    ds18b20_romcodes[ui].bytes[2],
                    ds18b20_romcodes[ui].bytes[3],
                    ds18b20_romcodes[ui].bytes[4],
                    ds18b20_romcodes[ui].bytes[5],
                    ds18b20_romcodes[ui].bytes[6]);
            trip_mod(TRIP_DS18X20_MISSING, ui);
        }
    }

    return 0;
}

static ALWAYSINLINE int32_t
init_battery_bypass_in(void)
{
    clear_wdt();
    PIN_SET_DIGITAL_IN(pin_batt_bypass);
    return 0;
}

static ALWAYSINLINE int32_t
init_devices(void)
{
    clear_wdt();

    init_relays();
    init_battery_bypass_in();
    REPORT_ON_ERR(init_leds(),      REP_WARNING, "init_leds");
    REPORT_ON_ERR(init_serial(),    REP_WARNING, "init_serial");
    REPORT_ON_ERR(init_nokias(),    REP_WARNING, "init_nokias");
    REPORT_ON_ERR(init_cans(),      REP_WARNING, "init_cans");
    IF_ERR(init_ltcs(),             REP_WARNING, "init_ltcs")
        trip_nomod(TRIP_OTHER);
    IF_ERR(init_adcs(),             REP_WARNING, "init_adcs")
        trip_nomod(TRIP_OTHER);
    IF_ERR(init_ds18b20s(),         REP_WARNING, "init_ds18b20s")
        trip_nomod(TRIP_OTHER);

    return 0;
}

static void
doVoltages(void)
{
    static enum voltageConversionState {
        VCONVERT_NONE,
        VCONVERT_OPENWIRE,
        VCONVERT_NORMAL,
    } vConvertStatus = VCONVERT_NONE;
    static uint32_t last_openWireConversion = 0;
    static uint32_t last_voltageConversion = 0;

    clear_wdt();

    /* Start voltage conversion if none in progress */
    if (ticksToSecs(ReadCoreTimer() - last_openWireConversion) > interval_get_ow_voltages) {
        uint32_t ui;
        init_ltcs();
        IF_ERR(ltcp->op->startOpenWireConversion(ltcp), REP_CRITICAL,
                "LTC OPEN WIRE CONVERSION FAILED")
            trip_nomod(TRIP_OTHER);
        delay_ms(16);
        IF_ERR(ltcp->op->readVolts(ltcp, openWireVoltages), REP_CRITICAL,
                "LTC READVOLTS FAILED")
           trip_nomod(TRIP_OTHER);
        for (ui = 0; ui < module_count; ui++) {
            clear_wdt();
            if (openWireVoltages[ui] < 0.5) {
                if (ui+1 != battBypass) {
                    REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d DISCONNECTED", ui);
                    trip_mod(TRIP_OTHER, ui);
                } else {
                    REPORT(REP_WARNING, "MODULE %d BYPASSED", ui);
                }
            }
        }
        last_openWireConversion = ReadCoreTimer();
    } else if (ticksToSecs(ReadCoreTimer() - last_voltageConversion) > interval_get_voltages) {
        uint32_t ui;
        double sum;
        init_ltcs();
        IF_ERR(ltcp->op->startVoltageConversion(ltcp), REP_CRITICAL,
                "LTC VOLTAGE CONVERSION FAILED")
            trip_nomod(TRIP_OTHER);
        delay_ms(16);
        IF_ERR(ltcp->op->readVolts(ltcp, voltages), REP_CRITICAL,
                        "LTC READVOLTS FAILED")
           trip_nomod(TRIP_OTHER);
        /* Check for over/under-voltage */
        for (ui = 0; ui < module_count; ui++) {
            clear_wdt();
            sum += voltages[ui];
            if (voltages[ui] > over_voltage) {
                REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d OVER VOLTAGE", ui);
                trip_mod(TRIP_OVER_VOLTAGE, ui);
            } else if (voltages[ui] < under_voltage) {
                if (ui+1 != battBypass) {
                    REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d UNDER VOLTAGE", ui);
                    trip_mod(TRIP_UNDER_VOLTAGE, ui);
                } else {
                    REPORT(REP_WARNING, "MODULE %d BYPASSED", ui);
                }
            }
        }
        wh_battery += (sum*ticksToSecs(ReadCoreTimer()-last_voltageConversion))/3600;
        last_voltageConversion = ReadCoreTimer();
    }

/*
 *     if (vConvertStatus == VCONVERT_NONE) {
 *        if (ticksToSecs(ReadCoreTimer() - last_openWireConversion) > INTERVAL_GET_OW_VOLTAGES) {
 *            IF_ERR(ltcp->op->startOpenWireConversion(ltcp), REP_CRITICAL,
 *                    "LTC OPEN WIRE CONVERSION FAILED")
 *                trip_nomod(TRIP_OTHER);
 *            last_openWireConversion = ReadCoreTimer();
 *            vConvertStatus = VCONVERT_OPENWIRE;
 *        } else if (ticksToSecs(ReadCoreTimer() - last_voltageConversion) > INTERVAL_GET_VOLTAGES) {
 *            IF_ERR(ltcp->op->startVoltageConversion(ltcp), REP_CRITICAL,
 *                    "LTC VOLTAGE CONVERSION FAILED")
 *                trip_nomod(TRIP_OTHER);
 *            last_voltageConversion = ReadCoreTimer();
 *            vConvertStatus = VCONVERT_NORMAL;
 *        }
 *    }
 *
 *    if (vConvertStatus == VCONVERT_OPENWIRE &&
 *            ticksToSecs(ReadCoreTimer() - last_openWireConversion) > 16E-3) {
 *        uint32_t ui;
 *        IF_ERR(ltcp->op->readVolts(ltcp, voltages), REP_CRITICAL,
 *                "LTC READVOLTS FAILED")
 *           trip_nomod(TRIP_OTHER);
 *        for (ui = 0; ui < MODULE_COUNT; ui++) {
 *            ClearWDT();
 *            if (openWireVoltages[ui] < 0.5) {
 *                if (ui+1 != battBypass) {
 *                    REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d DISCONNECTED", ui);
 *                    trip_mod(TRIP_OTHER, ui);
 *                } else {
 *                    REPORT(REP_WARNING, "MODULE %d BYPASSED", ui);
 *                }
 *            }
 *        }
 *        vConvertStatus = VCONVERT_NONE;
 *    } else if (vConvertStatus == VCONVERT_NORMAL &&
 *            ticksToSecs(ReadCoreTimer() - last_voltageConversion) > 16E-3) {
 *        uint32_t ui;
 *        double sum;
 *        IF_ERR(ltcp->op->readVolts(ltcp, voltages), REP_CRITICAL,
 *                        "LTC READVOLTS FAILED")
 *           trip_nomod(TRIP_OTHER);
 */
    /* Check for over/under-voltage */
/*
 *         for (ui = 0; ui < MODULE_COUNT; ui++) {
 *            ClearWDT();
 *            sum += voltages[ui];
 *            if (voltages[ui] > OVER_VOLTAGE) {
 *                REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d OVER VOLTAGE", ui);
 *                trip_mod(TRIP_OVER_VOLTAGE, ui);
 *            } else if (voltages[ui] < UNDER_VOLTAGE) {
 *                if (ui+1 != battBypass) {
 *                    REPORT_ERR(REP_EMERGENCY, -ETRIP, "MODULE %d UNDER VOLTAGE", ui);
 *                    trip_mod(TRIP_UNDER_VOLTAGE, ui);
 *                } else {
 *                    REPORT(REP_WARNING, "MODULE %d BYPASSED", ui);
 *                }
 *            }
 *        }
 *        wh_battery += (sum*ticksToSecs(ReadCoreTimer()-last_voltageConversion))/3600;
 *        vConvertStatus = VCONVERT_NONE;
 *    }
 */
}

static void
doCurrents(void)
{
    static uint32_t last_currents = 0;

    ClearWDT();

    if (ticksToSecs(ReadCoreTimer() - last_currents) > interval_get_currents) {
        IF_ERR(adcp->op->convertAndReadVolts(adcp, rawCurrents),
                        REP_CRITICAL, "Current acquisition failed")
            trip_nomod(TRIP_ADC_FAILURE);
        currentBattery  = voltageToCurrent(rawCurrents[i_sensor_batt]);
        currentArray    = voltageToCurrent(rawCurrents[i_sensor_array]);

        /* Check for over/under-current */
        if (currentBattery > over_current_batt_discharge_a) {
            REPORT_ERR(REP_EMERGENCY, -ETRIP, "OVER CURRENT DISCHRG BATT");
            trip_nomod(TRIP_OVER_CURRENT_DISCHRG);
        } else if (currentBattery < over_current_batt_charge_a) {
            REPORT_ERR(REP_EMERGENCY, -ETRIP, "OVER CURRENT CHRG BATT");
            trip_nomod(TRIP_OVER_CURRENT_CHRG);
        }

        /*
        if (currentArray < UNDER_CURRENT_ARRAY_A) {
            REPORT_ERR(REP_EMERGENCY, -ETRIP, "OVER CURRENT DISCHRG ARRAY");
            trip_nomod(TRIP_OVER_CURRENT_DISCHRG);
        } else if (currentArray > OVER_CURRENT_ARRAY_A) {
            REPORT_ERR(REP_EMERGENCY, -ETRIP, "OVER CURRENT CHRG ARRAY");
            trip_nomod(TRIP_OVER_CURRENT_CHRG);
        }
         */

        cc_battery      += (ticksToSecs(ReadCoreTimer() - last_currents)*currentBattery)/3600;
        cc_array        += (ticksToSecs(ReadCoreTimer() - last_currents)*currentArray)/3600;
        last_currents = ReadCoreTimer();
    }
}

static void
doTemperatures(void)
{
    static int32_t errno;
    static enum tempConversionState {
        TEMP_NONE,
        TEMP_BUSY,
    } tConvertStatus = TEMP_NONE;
    static uint32_t last_tempConvert = 0;

    ClearWDT();

    if (tConvertStatus == TEMP_NONE &&
            ticksToSecs(ReadCoreTimer() - last_tempConvert) > interval_get_temps) {
        /* Start next temperature conversion */
        IF_ERR(errno = ds_start_temp_conversion(dsp, ALL_DEVICES), REP_EMERGENCY,
                errno == -ENODEV ? "OW BUS FAILURE" : "TEMP CONVERT FAILED")
            trip_nomod(errno == -ENODEV ? TRIP_DS18X20_MISSING :
                TRIP_OW_BUS_FAILURE);
        last_tempConvert = ReadCoreTimer();
        tConvertStatus = TEMP_BUSY;
    } else if (tConvertStatus == TEMP_BUSY &&
            ticksToSecs(ReadCoreTimer() - last_tempConvert) > 760E-3) {
        uint32_t ui;
        /* Get temperature readings */
        for (ui = 0; ui < ARRAY_SIZE(ds18b20_romcodes); ++ui) {
            IF_ERR(temperatures[ui] = ds_read_temp(dsp, ds18b20_romcodes[ui]),
                    REP_EMERGENCY, "FAILED READTEMP %d", ui) {
                if (likely(temperatures[ui] == -ECRC))
                    trip_mod(TRIP_OW_BUS_FAILURE, ui);
                else
                    trip_mod(TRIP_OTHER, ui);
            }
            /* check for over/under temp */
            if (temperatures[ui] > over_temp_c) {
                REPORT_ERR(REP_EMERGENCY, -ETRIP, "SENSOR %d OVER TEMP", ui);
                trip_mod(TRIP_OVER_TEMP, ui);
            } else if (temperatures[ui] < under_temp_c) {
                REPORT_ERR(REP_EMERGENCY, -ETRIP, "SENSOR %d UNDER TEMP", ui);
                trip_mod(TRIP_UNDER_TEMP, ui);
            } else {
                break;
            }
        }
        tConvertStatus = TEMP_NONE;
    }
}

static void
doSaveFlash(void)
{
    static uint32_t last_saveFlash = 0;

    ClearWDT();

    if (ticksToSecs(ReadCoreTimer() - last_saveFlash) > interval_save_flash) {
        saveFlashNow();
        last_saveFlash = ReadCoreTimer();
    }
}

static void
doCanTx(void)
{
    static uint32_t last_sendHeartbeat      = 0;
    static uint32_t last_sendUptime         = 0;
    static uint32_t last_sendLastReset      = 0;
    static uint32_t last_sendLastTrip       = 0;
    static uint32_t last_sendBattBypass     = 0;
    static uint32_t last_sendCurrents       = 0;
    static uint32_t last_sendVoltages       = 0;
    static uint32_t last_sendOwVoltages     = 0;
    static uint32_t last_sendCcArray        = 0;
    static uint32_t last_sendCcBatt         = 0;
    static uint32_t last_sendMpptRequests   = 0;
    static uint32_t last_sendTemps          = 0;
    static uint32_t last_sendWhBatt         = 0;
    static uint32_t last_sendTripPtCurrent  = 0;
    static uint32_t last_sendTripPtVoltage  = 0;
    static uint32_t last_sendTripPtTemp     = 0;

    uint32_t core_timer = ReadCoreTimer();

    ClearWDT();

    if (ticksToSecs(core_timer - last_sendHeartbeat) > interval_tx_heartbeat) {
        struct can_bms_tx_heartbeat hb = {
            .bmsStr = "bms0",
        };
        ccTx(ADDR_BMS_TX_HEARTBEAT, hb);
        last_sendHeartbeat = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendUptime) > interval_tx_uptime) {
        struct can_bms_tx_uptime canUptime = {
            .seconds = uptime,
        };
        ccTx(ADDR_BMS_TX_UPTIME, canUptime);
        last_sendUptime = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendLastReset) > interval_tx_lastreset) {
        struct can_bms_tx_last_reset lrc = {
            .lastResetCode  = last_reset_cause,
            .reserved       = 0,
        };
        ccTx(ADDR_BMS_TX_LAST_RESET, lrc);
        last_sendLastReset = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendLastTrip) > interval_tx_lasttrip &&
            last_trip_module != UNINIT) {
        struct can_bms_tx_last_trip lt = {
            .module     = last_trip_module,
            .tripCode   = last_tripcode,
        };
        ccTx(ADDR_BMS_TX_LAST_TRIP, lt);
        last_sendLastTrip = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendBattBypass) > interval_tx_battbypass) {
        struct can_bms_tx_batt_bypass bb = {
            .module = battBypass,
        };
        ccTx(ADDR_BMS_TX_BATT_BYPASS, bb);
        last_sendBattBypass = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendCurrents) > interval_tx_currents &&
            (!FEQ(currentArray, UNINIT) || !FEQ(currentBattery, UNINIT))) {
        struct can_bms_tx_current cur = {
            .array      = currentArray,
            .battery    = currentBattery,
        };
        ccTx(ADDR_BMS_TX_CURRENT, cur);
        last_sendCurrents = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendVoltages) > interval_tx_voltages &&
            !FEQ(voltages[0],UNINIT)) {
        uint32_t ui;
        for (ui = 0; ui < module_count; ++ui) {
            struct can_bms_tx_voltage v = {
                .module     = ui,
                .voltage    = voltages[ui],
            };
            ccTx(ADDR_BMS_TX_VOLTAGE, v);
        }
        last_sendVoltages = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendOwVoltages) > interval_tx_ow_voltages &&
            !FEQ(openWireVoltages[0], UNINIT)) {
        uint32_t ui;
        for (ui = 0; ui < module_count; ++ui) {
            struct can_bms_tx_owVoltage owv = {
                .module     = ui,
                .owVoltage  = openWireVoltages[ui],
            };
            ccTx(ADDR_BMS_TX_OWVOLTAGE, owv);
        }
        last_sendOwVoltages = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendTemps) > interval_tx_temps &&
            !FEQ(temperatures[0], UNINIT)) {
        uint32_t ui;
        for (ui = 0; ui < ARRAY_SIZE(temperatures); ++ui) {
            struct can_bms_tx_temp t = {
                .sensor = ui,
                .temp   = temperatures[ui],
            };
            ccTx(ADDR_BMS_TX_TEMP, t);
        }
        last_sendTemps = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendCcArray) > interval_tx_cc_array) {
        struct can_bms_tx_current_count_array cc = {
            .count = cc_array,
        };
        ccTx(ADDR_BMS_TX_CC_ARRAY, cc);
        last_sendCcArray = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendCcBatt) > interval_tx_cc_battery) {
        struct can_bms_tx_current_count_battery cc = {
            .count = cc_battery,
        };
        ccTx(ADDR_BMS_TX_CC_BATT, cc);
        last_sendCcBatt = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendWhBatt) > interval_tx_wh_batt) {
        struct can_bms_tx_wh_batt wh = {
            .Wh = wh_battery,
        };
        ccTx(ADDR_BMS_TX_WH_BATT, wh);
        last_sendWhBatt = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendTripPtCurrent) > interval_tx_trip_pt_current) {
        struct can_bms_tx_trip_pt_current tpc = {
            .low    = over_current_batt_discharge_a,
            .high   = over_current_batt_charge_a,
        };
        ccTx(ADDR_BMS_TX_TRIP_PT_CURRENT, tpc);
        last_sendTripPtCurrent = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendTripPtVoltage) > interval_tx_trip_pt_voltage) {
        struct can_bms_tx_trip_pt_voltage tp = {
            .low    = under_voltage,
            .high   = over_voltage,
        };
        ccTx(ADDR_BMS_TX_TRIP_PT_VOLTAGE, tp);
        last_sendTripPtVoltage = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendTripPtTemp) > interval_tx_trip_pt_temp) {
        struct can_bms_tx_trip_pt_temp tp = {
            .low    = under_temp_c,
            .high   = over_temp_c,
        };
        ccTx(ADDR_BMS_TX_TRIP_PT_VOLTAGE, tp);
        last_sendTripPtTemp = core_timer;
    }

    if (ticksToSecs(core_timer - last_sendMpptRequests) > interval_tx_mpptrequests) {
        mpptCanp->op->tx(mpptCanp, mppt_can_tx_chn, STANDARD_ID,
                ADDR_MPPT1_RX_BASE, 0, 1, NULL, 0);
        mpptCanp->op->tx(mpptCanp, mppt_can_tx_chn, STANDARD_ID,
                ADDR_MPPT2_RX_BASE, 0, 1, NULL, 0);
        mpptCanp->op->tx(mpptCanp, mppt_can_tx_chn, STANDARD_ID,
                ADDR_MPPT3_RX_BASE, 0, 1, NULL, 0);

        last_sendMpptRequests = core_timer;
    }
}

static void
doCanRx(void)
{
    union can_anyFrame frame;
    uint32_t sid;
    uint32_t ui;

    ClearWDT();

    for (ui = 0;
            ui < 32 && mpptCanp->op->rx(mpptCanp, mppt_can_rx_chn, &sid, &frame) > 0;
            ++ui) {
        static uint32_t last_mppt1rx = 0;
        static uint32_t last_mppt2rx = 0;
        static uint32_t last_mppt3rx = 0;

        commonCanp->op->tx(commonCanp, common_can_tx_chn, STANDARD_ID,
                (uint16_t)sid, 0, 0, &frame, sizeof(frame));
        switch (sid) {
        case ADDR_MPPT1_TX_FRAME:
            cc_mppt1 += ticksToSecs(ReadCoreTimer() - last_mppt1rx) * frame.mppt.tx.frame.iIn;
            last_mppt1rx = ReadCoreTimer();
            break;
        case ADDR_MPPT2_TX_FRAME:
            cc_mppt2 += ticksToSecs(ReadCoreTimer() - last_mppt2rx) * frame.mppt.tx.frame.iIn;
            last_mppt2rx = ReadCoreTimer();
            break;
        case ADDR_MPPT3_TX_FRAME:
            cc_mppt3 += ticksToSecs(ReadCoreTimer() - last_mppt2rx) * frame.mppt.tx.frame.iIn;
            last_mppt3rx = ReadCoreTimer();
            break;
        default:
            REPORT(REP_WARNING, "unrecognized CAN addr 0x%0X", sid);
            break;
        }
    }

    ClearWDT();

    for (ui = 0;
            ui < 32 && commonCanp->op->rx(commonCanp, common_can_tx_chn, &sid, &frame) > 0;
            ++ui) {
        switch (sid) {
        case ADDR_BMS_RX_TRIP:
            trip_mod(frame.bms.rx.trip.tripCode, frame.bms.rx.trip.module);
            break;
        case ADDR_BMS_RX_RESET_CC_BATT:
            cc_battery = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_CC_ARRAY:
            cc_array = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_CC_MPPT1:
            cc_mppt1 = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_CC_MPPT2:
            cc_mppt2 = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_CC_MPPT3:
            cc_mppt3 = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_WH:
            wh_battery  = 0;
            wh_mppt1in  = 0;
            wh_mppt1out = 0;
            wh_mppt2in  = 0;
            wh_mppt2out = 0;
            wh_mppt3in  = 0;
            wh_mppt3out = 0;
            saveFlashNow();
            break;
        case ADDR_BMS_RX_RESET_ALL:
            cc_array    = 0;
            cc_battery  = 0;
            cc_mppt1    = 0;
            cc_mppt2    = 0;
            cc_mppt3    = 0;
            wh_battery  = 0;
            wh_mppt1in  = 0;
            wh_mppt1out = 0;
            wh_mppt2in  = 0;
            wh_mppt2out = 0;
            wh_mppt3in  = 0;
            wh_mppt3out = 0;
            saveFlashNow();
            break;
        default:
            REPORT(REP_WARNING, "unrecognized CAN addr 0x%0X", sid);
            break;
        }
    }
}

static void
loadFlash(void)
{
    struct flashData fd, fd_ones;

    REPORT_ON_ERR(readFlash(&fd,sizeof(fd)), REP_WARNING, "readFlash error");
    memset(&fd_ones, (int)0xFFFFFFFF, sizeof(fd_ones));
    if (!memcmp(&fd, &fd_ones, sizeof(fd))) {
        REPORT(REP_DEBUG, "flash uninitialized");
        memset(&fd, 0, sizeof(fd));
    } else {
        last_trip_module = fd.last_trip_module;
        last_tripcode   = fd.last_tripcode;
        strncpy(lastTrip_file, fd.last_trip_file, ARRAY_SIZE(lastTrip_file)-1);
        last_trip_line   = fd.last_trip_line;
        last_trip_temp   = fd.last_trip_temp;
        cc_battery      = fd.cc_battery;
        cc_array        = fd.cc_array;
        cc_mppt1        = fd.cc_mppt1;
        cc_mppt2        = fd.cc_mppt2;
        cc_mppt3        = fd.cc_mppt3;
        wh_battery      = fd.wh_battery;
        wh_mppt1in      = fd.wh_mppt1in;
        wh_mppt1out     = fd.wh_mppt1out;
        wh_mppt2in      = fd.wh_mppt2in;
        wh_mppt2out     = fd.wh_mppt2out;
        wh_mppt3in      = fd.wh_mppt3in;
        wh_mppt3out     = fd.wh_mppt3out;
    }
}

static void
doSerTx(void)
{
    static uint32_t last_serTx = 0;

    uint32_t ui;
    float sum;
    float min;
    float max;

    clear_wdt();

    if (ticksToSecs(ReadCoreTimer() - last_serTx) < interval_serial_tx)
        return;

    nu32_serp->op->printf(nu32_serp, "t uptime=         %f\r\n", uptime);

    nu32_serp->op->printf(nu32_serp, "~~~~~LAST TRIP BEGIN~~~~~\r\n");
    nu32_serp->op->printf(nu32_serp, "\tlastTrip_module=%d\r\n", last_trip_module);
    nu32_serp->op->printf(nu32_serp, "\tlastTrip_temp=  %f\r\n", last_trip_temp);
    nu32_serp->op->printf(nu32_serp, "~~~~~LAST TRIP END~~~~~\r\n");

    nu32_serp->op->printf(nu32_serp, "t lastResetCause= %d\r\n", last_reset_cause);
    nu32_serp->op->printf(nu32_serp, "t battBypass=     %u\r\n", battBypass);
    nu32_serp->op->printf(nu32_serp, "t currentBattery= %f\r\n", currentBattery);
    nu32_serp->op->printf(nu32_serp, "t currentArray=   %f\r\n", currentArray);
    nu32_serp->op->printf(nu32_serp, "t cc_battery=     %f\r\n", cc_battery);
    nu32_serp->op->printf(nu32_serp, "t cc_array=       %f\r\n", cc_array);
    nu32_serp->op->printf(nu32_serp,
                                     "t cc_mppt1=       %f\r\n"
                                     "t cc_mppt2=       %f\r\n"
                                     "t cc_mppt3=       %f\r\n",
                            cc_mppt1,
                            cc_mppt2,
                            cc_mppt3);
    nu32_serp->op->printf(nu32_serp, "t wh_battery=     %f\r\n", wh_battery);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt1in=     %f\r\n", wh_mppt1in);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt1out=    %f\r\n", wh_mppt1out);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt2in=     %f\r\n", wh_mppt2in);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt2out=    %f\r\n", wh_mppt2out);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt3in=     %f\r\n", wh_mppt3in);
    nu32_serp->op->printf(nu32_serp, "t wh_mppt3out=    %f\r\n", wh_mppt3out);

    nu32_serp->op->printf(nu32_serp, "~~~~~VOLTAGES BEGIN~~~~~\r\n");
    for (sum = 0, ui = 0; ui < ARRAY_SIZE(voltages); ++ui) {
        clear_wdt();
        nu32_serp->op->printf(nu32_serp, "\t%02u: %f\r\n", ui, voltages[ui]);
        if (ui < module_count)
            sum += voltages[ui];
    }
    nu32_serp->op->printf(nu32_serp, "\tVOLTAGE TOTAL(0-%u): %f\r\n", module_count-1, sum);
    nu32_serp->op->printf(nu32_serp, "~~~~~VOLTAGES END~~~~~\r\n");

    nu32_serp->op->printf(nu32_serp, "~~~~~OPEN WIRE VOLTAGES BEGIN~~~~~\r\n");
    for (sum = 0, ui = 0; ui < ARRAY_SIZE(openWireVoltages); ++ui) {
        clear_wdt();
        nu32_serp->op->printf(nu32_serp, "\t%02u: %f\r\n", ui, openWireVoltages[ui]);
        if (ui < module_count)
            sum += openWireVoltages[ui];
    }
    nu32_serp->op->printf(nu32_serp, "\tOPEN WIRE VOLTAGE TOTAL(0-%u): %f\r\n", module_count-1, sum);
    nu32_serp->op->printf(nu32_serp, "~~~~~OPEN WIRE VOLTAGES END~~~~~\r\n");

    nu32_serp->op->printf(nu32_serp, "~~~~~TEMPERATURES BEGIN~~~~~\r\n");
    for (sum=0, min=UNINIT, max=-UNINIT, ui = 0; ui < ARRAY_SIZE(temperatures); ++ui) {
        clear_wdt();
        nu32_serp->op->printf(nu32_serp, "\t%02u: %f\r\n", ui, temperatures[ui]);
        min = MIN(min, temperatures[ui]);
        max = MAX(max, temperatures[ui]);
        sum += temperatures[ui];
    }
    nu32_serp->op->printf(nu32_serp, "\tTEMP MIN: %f\r\n", min);
    nu32_serp->op->printf(nu32_serp, "\tTEMP AVG: %f\r\n", sum/ARRAY_SIZE(temperatures));
    nu32_serp->op->printf(nu32_serp, "\tTEMP MAX: %f\r\n", max);
    nu32_serp->op->printf(nu32_serp, "~~~~~TEMPERATURES END~~~~~\r\n");

    last_serTx = ReadCoreTimer();
}

int32_t
main(void)
{
    /* timeout setting is in main.h */
    EnableWDT();

    /* allow clearing of wdt with CLEARWDT() */
    enableClearWdt();

    clear_wdt();

    PIN_SET_DIGITAL_OUT(pin_main_relay);
    PIN_SET_DIGITAL_OUT(pin_array_relay);

    PIN_CLEAR(pin_main_relay);
    PIN_CLEAR(pin_array_relay);

    delay(.1);

    /* setup clock, interrupts, NU32 LED pinouts, and switch input */
    nu32_init(bms_sys_clk_hz);

    REPORT_ON_ERR(init_devices(), REP_WARNING, "init_devices failed");

    get_last_reset_cause();

    clear_wdt();

    loadFlash();

    /* report last trip
     * use reportf here so we can give the lastTrip_file and lastTrip_line
     *  rather than this file and line, which are much less useful in debugging
     *  trips
     */
    reportf(lastTrip_file, last_trip_line, REP_INFO, ENONE, NULL,
                (last_tripcode == TRIP_UNDER_TEMP || last_tripcode == TRIP_OVER_TEMP) ?
                    "mod %d,code %d(%s),t%f" : "mod %d,code %d(%s)",
                last_trip_module, last_tripcode,
                last_tripcode >= ARRAY_SIZE(tripcode_s) ? "?" : tripcode_s[last_tripcode],
                last_trip_temp);

    WriteCoreTimer(0);

    ClearWDT();

    while (1) {
        static uint32_t prevBattBypass = 0;
        static uint32_t last_uptime = 0;

        ClearWDT();

        uptime += ticksToSecs(ReadCoreTimer() - last_uptime);
        last_uptime = ReadCoreTimer();

        nokia_goto_xy(dp2, 0, 1);
        nokia_printf(dp2, "V:%0.9f", voltages[31]);
        nokia_goto_xy(dp2, 0, 2);
        nokia_printf(dp2, "T:%0.9f", temperatures[31]);
        nokia_goto_xy(dp2, 0, 3);
        nokia_printf(dp2, "I:%0.9f", currentBattery);

        battBypass = PIN_READ(pin_batt_bypass);
        if (battBypass != prevBattBypass) {
            nokia_clear(dp2);
            nokia_goto_xy(dp2, 0, 0);
            nokia_printf(dp2, "batt bypass %d", battBypass);
        }
        prevBattBypass = battBypass;

        doVoltages();
        doCurrents();
        doTemperatures();
        doSaveFlash();
        doCanTx();
        doCanRx();
        doSerTx();
    }

    exit(EXIT_SUCCESS); /* should never get here */
}

#endif

#endif /* NU_BPS_H */
