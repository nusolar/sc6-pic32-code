#ifndef NU_LTC6803_H
#define NU_LTC6803_H

#include "compiler.h"
#include "nu_types.h"
#include "spi.h"
#include "utility.h"

enum ltc6803_cfgr0 {
    /** watchdog timer */
    LTC6803_WDT         = 1<<7,

    /** 0 = open drain output (pin is pulled to V-), 1 = high impedence */
    LTC6803_GPIO2       = 1<<6,

    /** Behaves same as GPIO1 */
    LTC6803_GPIO1       = 1<<5,

    /** Level Polling Mode; 0 = toggle polling (default), 1 = level polling */
    LTC6803_LVLPL       = 1<<4,

    /** 0 = 12-cell mode (default), 1 = 10-cell mode */
    LTC6803_CELL10      = 1<<3,
    
    LTC6803_CDC2        = 1<<2,
    LTC6803_CDC1        = 1<<1,
    LTC6803_CDC0        = 1,
    LTC6803_CDC_VAL0    = 0,
    LTC6803_CDC_VAL1,
    LTC6803_CDC_VAL2,
    LTC6803_CDC_VAL3,
    LTC6803_CDC_VAL4,
    LTC6803_CDC_VAL5,
    LTC6803_CDC_VAL6,
    LTC6803_CDC_VAL7,

    /** (Comparator Off) Standby Mode */
    LTC6803_CDC_STANDBY = LTC6803_CDC_VAL0,
    /** Comparator Off; Vref not powered down between msmts; 13ms cell voltage
     * msmt time */
    LTC6803_CDC_MSMTONLY,
    /** 13ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    LTC6803_CDC_13ms,
    /** 130ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    LTC6803_CDC_130ms,
    /** 500ms UV/OV comparator period; Vref not powered down between msmts; 13ms
     * cell voltage msmt time */
    LTC6803_CDC_500ms,
    /** 130ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    LTC6803_CDC_130ms_VrefOff,
    /** 500ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    LTC6803_CDC_500ms_VrefOff,
    /** 2000ms UV/OV comparator period; Vref powered down between msmts; 21ms
     * cell voltage msmt time */
    LTC6803_CDC_2000ms_VrefOff
};

/**
 * Config Register 1
 * DCCx = Discharge Cell x; x = 1-12; 0 = turn off shorting switch for cell 'x'
 * (default), 1 = turn on shorting switch
 */
enum ltc6803_cfgr1 {
    LTC6803_DCC8 = 1<<7,
    LTC6803_DCC7 = 1<<6,
    LTC6803_DCC6 = 1<<5,
    LTC6803_DCC5 = 1<<4,
    LTC6803_DCC4 = 1<<3,
    LTC6803_DCC3 = 1<<2,
    LTC6803_DCC2 = 1<<1,
    LTC6803_DCC1 = 1,
};

enum ltc6803_cfgr2 {
    /** MCxI = Mask Cell x Interrupts; x = 1-12; 0 = enable interrupts for cell
     * 'x' (default), 1 = turn off interrupts and clear flags for cell 'x' */
    LTC6803_MC4I    = 1<<7,
    LTC6803_MC3I    = 1<<6,
    LTC6803_MC2I    = 1<<5,
    LTC6803_MC1O    = 1<<4,
    /** DCCx: see note for cfgr1 */
    LTC6803_DCC12   = 1<<3,
    LTC6803_DCC11   = 1<<2,
    LTC6803_DCC10   = 1<<1,
    LTC6803_DCC9    = 1,
};

enum ltc6803_cfgr3 {
    LTC6803_MC12I   = 1<<7,
    LTC6803_MC11I   = 1<<6,
    LTC6803_MC10I   = 1<<5,
    LTC6803_MC9I    = 1<<4,
    LTC6803_MC8I    = 1<<3,
    LTC6803_MC7I    = 1<<2,
    LTC6803_MC6I    = 1<<1,
    LTC6803_MC5I    = 1,
};

enum ltc6803_cfgr4 {
    /** V_UV = Undervoltage Comparison Voltage;
     * Comparison voltage = (V_UV-31)*16*1.5mV
     * It's better just to cast a uint or numeric literal to a BYTE for this
     * instead of setting these manually
     */
    LTC6803_VUV7    = 1<<7,
    LTC6803_VUV6    = 1<<6,
    LTC6803_VUV5    = 1<<5,
    LTC6803_VUV4    = 1<<4,
    LTC6803_VUV3    = 1<<3,
    LTC6803_VUV2    = 1<<2,
    LTC6803_VUV1    = 1<<1,
    LTC6803_VUV0    = 1,
};

enum ltc6803_cfgr5 {
    /** V_OV = Overvoltage Comparison Voltage;
     * Comparison voltage = (V_OV-31)*16*1.5mV
     * See note about cfgr4
     */
    LTC6803_VOV7    = 1<<7,
    LTC6803_VOV6    = 1<<6,
    LTC6803_VOV5    = 1<<5,
    LTC6803_VOV4    = 1<<4,
    LTC6803_VOV3    = 1<<3,
    LTC6803_VOV2    = 1<<2,
    LTC6803_VOV1    = 1<<1,
    LTC6803_VOV0    = 1,
};

struct ltc6803 {
    struct spi spi;
    u32 num_devices;
    struct ltc6803_flags {
        u32 mismatch_cfgs;
        u32 mismatch_pecs;
    } flags;
};

union ltc6803_cfg {
    struct ltc6803_cfg_registers {
        u8 cfgr0;
        u8 cfgr1;
        u8 cfgr2;
        u8 cfgr3;
        u8 cfgr4;
        u8 cfgr5;
    } registers;
    PACKED struct ltc6803_cfg_bits {
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
        u8 vuv;
        u8 vov;
    } bits;
    u8 bytes[6];
};

STATIC_ASSERT(6 == sizeof(union BpsConfig), SIZE_MISMATCH);

s32
ltc6803_write_cfgs(struct ltc6803 *l, const union ltc6803_cfg *cfgs);

s32
ltc6803_read_cfgs(struct ltc6803 *l, union ltc6803_cfg *dst);

s32
ltc6803_cfgs_match(struct ltc6803 *l, const union ltc6803_cfg *cfgs);

s32
ltc6803_start_voltage_conversion(struct ltc6803 *l);

s32
ltc6803_start_open_wire_conversion(struct ltc6803 *l);

s32
ltc6803_read_voltages(struct ltc6803 *l, float *dst);

s32
ltc6803_post(struct ltc6803 *l);

s32
ltc6803_check_open_wire(struct ltc6803 *l, u16 *open_wire_flags, const float *voltages);

int32_t
ltc6803_new (struct ltc6803 *self, SpiChannel _chn,
        IoPortId _csPinLtr,         uint32_t _csPinNum,
        uint32_t _numDevices,   const union ltc6803_cfg *cfgs);

PURE u8
ltc6803_convert_uv_limit(double vuv);

PURE u8
ltc6803_convert_ov_limit(double vov);

#endif
