#ifndef __NU_LTC6803_H
#define __NU_LTC6803_H

#include <stdint.h>
#include "errorcodes.h"
#include "spi.h"
#include "timer.h"
#include "utility.h"

enum cfgr0 {
    /** watchdog timer */
    WDT         = 1<<7,

    /** 0 = open drain output (pin is pulled to V-), 1 = high impedence */
    GPIO2       = 1<<6,

    /** Behaves same as GPIO1 */
    GPIO1       = 1<<5,

    /** Level Polling Mode; 0 = toggle polling (default), 1 = level polling */
    LVLPL       = 1<<4,

    /** 0 = 12-cell mode (default), 1 = 10-cell mode */
    CELL10      = 1<<3,
    
    CDC2        = 1<<2,
    CDC1        = 1<<1,
    CDC0        = 1,
    CDC_VAL0    = 0,
    CDC_VAL1,
    CDC_VAL2,
    CDC_VAL3,
    CDC_VAL4,
    CDC_VAL5,
    CDC_VAL6,
    CDC_VAL7,

    /** (Comparator Off) Standby Mode */
    CDC_STANDBY = CDC_VAL0,
    /** Comparator Off; Vref not powered down between msmts; 13ms cell voltage
     * msmt time */
    CDC_MSMTONLY,
    /** 13ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    CDC_13ms,
    /** 130ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    CDC_130ms,
    /** 500ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    CDC_500ms,
    /** 130ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    CDC_130ms_VrefOff,
    /** 500ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    CDC_500ms_VrefOff,
    /** 2000ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    CDC_2000ms_VrefOff
};

/**
 * Config Register 1
 * DCCx = Discharge Cell x; x = 1-12; 0 = turn off shorting switch for cell 'x'
 * (default), 1 = turn on shorting switch
 */
enum cfgr1 {
    DCC8 = 1<<7,
    DCC7 = 1<<6,
    DCC6 = 1<<5,
    DCC5 = 1<<4,
    DCC4 = 1<<3,
    DCC3 = 1<<2,
    DCC2 = 1<<1,
    DCC1 = 1,
};

enum cfgr2 {
    /** MCxI = Mask Cell x Interrupts; x = 1-12; 0 = enable interrupts for cell
     * 'x' (default), 1 = turn off interrupts and clear flags for cell 'x' */
    MC4I    = 1<<7,
    MC3I    = 1<<6,
    MC2I    = 1<<5,
    MC1O    = 1<<4,
    /** DCCx: see note for cfgr1 */
    DCC12   = 1<<3,
    DCC11   = 1<<2,
    DCC10   = 1<<1,
    DCC9    = 1,
};

enum cfgr3 {
    MC12I   = 1<<7,
    MC11I   = 1<<6,
    MC10I   = 1<<5,
    MC9I    = 1<<4,
    MC8I    = 1<<3,
    MC7I    = 1<<2,
    MC6I    = 1<<1,
    MC5I    = 1,
};

enum cfgr4 {
    /** V_UV = Undervoltage Comparison Voltage;
     * Comparison voltage = (V_UV-31)*16*1.5mV
     * It's better just to cast a uint or numeric literal to a BYTE for this
     * instead of setting these manually
     */
    VUV7    = 1<<7,
    VUV6    = 1<<6,
    VUV5    = 1<<5,
    VUV4    = 1<<4,
    VUV3    = 1<<3,
    VUV2    = 1<<2,
    VUV1    = 1<<1,
    VUV0    = 1,
};

enum cfgr5 {
    /** V_OV = Overvoltage Comparison Voltage;
     * Comparison voltage = (V_OV-31)*16*1.5mV
     * See note about cfgr4
     */
    VOV7    = 1<<7,
    VOV6    = 1<<6,
    VOV5    = 1<<5,
    VOV4    = 1<<4,
    VOV3    = 1<<3,
    VOV2    = 1<<2,
    VOV1    = 1<<1,
    VOV0    = 1,
};

struct ltc6803 {
    const struct vtblLtc6803 *op;

    struct spiPort  spiPort;
    unsigned int    numDevices;

    unsigned int    flags_mismatchCfgs;
    unsigned int    flags_mismatchPecs;
};

union BpsConfig {
        union {
            struct {
                BYTE cfgr0;
                BYTE cfgr1;
                BYTE cfgr2;
                BYTE cfgr3;
                BYTE cfgr4;
                BYTE cfgr5;
            };
            struct {
                unsigned cdc    :3;
                unsigned cell10 :1;
                unsigned lvlpl  :1;
                unsigned gpio1  :1;
                unsigned gpio2  :1;
                unsigned wdt    :1;
                unsigned dcc1   :1;
                unsigned dcc2   :1;
                unsigned dcc3   :1;
                unsigned dcc4   :1;
                unsigned dcc5   :1;
                unsigned dcc6   :1;
                unsigned dcc7   :1;
                unsigned dcc8   :1;
                unsigned dcc9   :1;
                unsigned dcc10  :1;
                unsigned dcc11  :1;
                unsigned dcc12  :1;
                unsigned mc1i   :1;
                unsigned mc2i   :1;
                unsigned mc3i   :1;
                unsigned mc4i   :1;
                unsigned mc5i   :1;
                unsigned mc6i   :1;
                unsigned mc7i   :1;
                unsigned mc8i   :1;
                unsigned mc9i   :1;
                unsigned mc10i  :1;
                unsigned mc11i  :1;
                unsigned mc12i  :1;
                BYTE vuv;
                BYTE vov;
            } __attribute__((__packed__));
    };
    BYTE byteArr[6];
};

struct vtblLtc6803 {
    int32_t (*writeCfgs)                    (struct ltc6803 *self, const union BpsConfig *cfgs);
    int32_t (*readCfgs)                     (struct ltc6803 *self, union BpsConfig *cfgs);
    int32_t (*cfgsMatch)                    (struct ltc6803 *self, const union BpsConfig *cfgs);
    int32_t (*startVoltageConversion)       (struct ltc6803 *self);
    int32_t (*startOpenWireConversion)      (struct ltc6803 *self);
    int32_t (*readVolts)                    (struct ltc6803 *self, float *voltages);
    int32_t (*post)                         (struct ltc6803 *self);
    int32_t (*checkOpenWire)                (struct ltc6803 *self, uint16_t *openWire, const float *voltages);
};

int32_t
ltc6803_new (struct ltc6803 *self, SpiChannel _chn,
        IoPortId _csPinLtr,         uint32_t _csPinNum,
        uint32_t _numDevices,   const union BpsConfig *cfgs);

inline BYTE
convertUVLimit (double vuv) __attribute__((always_inline));

inline BYTE
convertOVLimit (double vov) __attribute__((always_inline));

#endif
