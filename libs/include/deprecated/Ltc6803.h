/**
 * @file    Ltc6803.h
 * @author  Chris Yungmann (christopheryungmann2010@u.northwestern.edu)
 * @date    December, 2011
 * @brief   Functions and data structures for LTC6803-3 Multicell Battery Stack Monitor support.
 * @sa      http://www.linear.com/product/LTC6803-1%20and%20-3
 */
 
#ifndef _SOLARSPI_H
#define _SOLARSPI_H

#include <plib.h>
#include <string.h> /* needed for memcpy */

extern const UINT8  BPS_SPI_CHANNEL;
extern const UINT8  LTC6803_COUNT;
extern const UINT8  NUM_MODULES;
extern const double VOLTAGE_OVER_LIMIT;
extern const double VOLTAGE_UNDER_LIMIT;
extern const double TEMP_OVER_LIMIT;   /* degrees Celsius */
extern const double CURRENT_OVER_DISCHARGE_LIMIT;
extern const double CURRENT_OVER_CHARGE_LIMIT;

extern const unsigned int MAXCFGSMATCHFAIL;
extern const unsigned int MAXADCMATCHFAIL;
extern const unsigned int MAXDAGNFAIL;

/**
 * @name ltc6803cmd
 * LTC6803 command codes
 * @{
 */
#define WRCFG               (0x01)      /**< Write Configuration Register Group */
#define RDCFG               (0x02)      /**< Read Configuration Register Group */
#define RDCV                (0x04)      /**< Read All Cell Voltage Group */
#define RDCVA               (0x06)      /**< Read Cell Voltages 1-4 */
#define RDCVB               (0x08)      /**< Read Cell Voltages 5-8 */
#define RDCVC               (0x0A)      /**< Read Cell Voltages 9-12 */
#define RDFLG               (0x0C)      /**< Read Flag Register Group */
#define RDTMP               (0x0E)      /**< Read Temperature Register Group */
#define STCVAD              (0x10)      /**< Start Cell Voltage ADC Conversion and Poll Status (All) */
    #define STCVAD_1            (STCVAD+1)  /**< Start Conversion for Cell 1 */
    #define STCVAD_2            (STCVAD+2)  /**< Start Conversion for Cell 2 */
    #define STCVAD_3            (STCVAD+3)  /**< Start Conversion for Cell 3 */
    #define STCVAD_4            (STCVAD+4)  /**< Start Conversion for Cell 4 */
    #define STCVAD_5            (STCVAD+5)  /**< Start Conversion for Cell 5 */
    #define STCVAD_6            (STCVAD+6)  /**< Start Conversion for Cell 6 */
    #define STCVAD_7            (STCVAD+7)  /**< Start Conversion for Cell 7 */
    #define STCVAD_8            (STCVAD+8)  /**< Start Conversion for Cell 8 */
    #define STCVAD_9            (STCVAD+9)  /**< Start Conversion for Cell 9 */
    #define STCVAD_10           (STCVAD+10) /**< Start Conversion for Cell 10 */
    #define STCVAD_11           (STCVAD+11) /**< Start Conversion for Cell 11 */
    #define STCVAD_12           (STCVAD+12) /**< Start Conversion for Cell 12 */
    #define STCVAD_CLR          (STCVAD+13) /**< Start Cell Conversion (Clear)? */
    #define STCVAD_SELFTEST1    (STCVAD+14) /**< Start Cell Conversion Self Test 1 */
    #define STCVAD_SELFTEST2    (STCVAD+15) /**< Start Cell Conversion Self Test 2 */
#define STOWAD              (0x20)      /**< Start Open-Wire ADC Conversions and Poll Status (All) */
    #define STOWAD_1            (STOWAD+1)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 1 */
    #define STOWAD_2            (STOWAD+2)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 2 */
    #define STOWAD_3            (STOWAD+3)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 3 */
    #define STOWAD_4            (STOWAD+4)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 4 */
    #define STOWAD_5            (STOWAD+5)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 5 */
    #define STOWAD_6            (STOWAD+6)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 6 */
    #define STOWAD_7            (STOWAD+7)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 7 */
    #define STOWAD_8            (STOWAD+8)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 8 */
    #define STOWAD_9            (STOWAD+9)  /**< Start Open-Wire ADC Conversions and Poll Status for Cell 9 */
    #define STOWAD_10           (STOWAD+10) /**< Start Open-Wire ADC Conversions and Poll Status for Cell 10 */
    #define STOWAD_11           (STOWAD+11) /**< Start Open-Wire ADC Conversions and Poll Status for Start Open-Wire ADC Conversions and Poll Status for Cell 11 */
    #define STOWAD_12           (STOWAD+12) /**< Cell 12 */
#define STTMPAD             (0x30)      /**< Start Temperature ADC Conversions and Poll Status (All) */
    #define STTMPAD_EXT1        (STTMPAD+1) /**< Start Temperature ADC Conversions and Poll Status for External1 */
    #define STTMPAD_EXT2        (STTMPAD+2) /**< Start Temperature ADC Conversions and Poll Status for External2 */
    #define STTMPAD_INTERNAL    (STTMPAD+3) /**< Start Temperature ADC Conversions and Poll Status for Internal */
    #define STTMPAD_SELFTEST1   (STTMPAD+4)
    #define STTMPAD_SELFTEST2   (STTMPAD+5)
#define PLADC               (0x40)      /**< Poll ADC Converter Status */
#define PLINT               (0x50)      /**< Poll Interrupt Status */
#define DAGN                (0x52)      /**< Start Diagnose and Poll Status */
#define RDDGNR              (0x54)      /**< Read Diagnostic Register */
#define STCVDC              (0x60)      /**< Start Cell Voltage ADC Conversions and Poll Status, with Discharge Permitted (All) */
    #define STCVDC_1            (STCVDC+1)  /**< Cell 1 */
    #define STCVDC_2            (STCVDC+2)
    #define STCVDC_3            (STCVDC+3)
    #define STCVDC_4            (STCVDC+4)
    #define STCVDC_5            (STCVDC+5)
    #define STCVDC_6            (STCVDC+6)
    #define STCVDC_7            (STCVDC+7)
    #define STCVDC_8            (STCVDC+8)
    #define STCVDC_9            (STCVDC+9)
    #define STCVDC_10           (STCVDC+10)
    #define STCVDC_11           (STCVDC+11)
    #define STCVDC_12           (STCVDC+12)
#define STOWDC              (0x70)      /**< Start Open-Wire ADC Conversions and Poll Status, with Discharge Permitted (All) */
    #define STOWDC_1             (STOWDC+1)
    #define STOWDC_2             (STOWDC+2)
    #define STOWDC_3             (STOWDC+3)
    #define STOWDC_4             (STOWDC+4)
    #define STOWDC_5             (STOWDC+5)
    #define STOWDC_6             (STOWDC+6)
    #define STOWDC_7             (STOWDC+7)
    #define STOWDC_8             (STOWDC+8)
    #define STOWDC_9             (STOWDC+9)
    #define STOWDC_10            (STOWDC+10)
    #define STOWDC_11            (STOWDC+11)
    #define STOWDC_12            (STOWDC+12)
/** @} */ /* end of group ltc6803cmd */

/**
 * @name cfgr0
 * Configuration register 0
 * @{
 */
#define WDT         (1<<7)  /**< Watchdog Timer */
#define GPIO2       (1<<6)  /**< 0 = open drain output (pin is pulled to V-), 1 = high impedence */
#define GPIO1       (1<<5)  /**< 0 = open drain output (pin is pulled to V-), 1 = high impedence */
#define LVLPL       (1<<4)  /**< Level Polling Mode; 0 = toggle polling (default), 1 = level polling */
#define CELL10      (1<<3)  /**< 0 = 12-cell mode (default), 1 = 10-cell mode */
#define CDC2        (1<<2)  /**< CDC[2]; Comparator Duty Cycle Bit 2 (high bit) */
#define CDC1        (1<<1)  /**< CDC[1]; Comparator Duty Cycle Bit 1 */
#define CDC0        (1<<0)  /**< CDC[0]; Comparator Duty Cycle Bit 0 */
    #define CDC_VAL0    (0)
    #define CDC_VAL1    (CDC0)
    #define CDC_VAL2    (CDC1)
    #define CDC_VAL3    (CDC1|CDC0)
    #define CDC_VAL4    (CDC2)
    #define CDC_VAL5    (CDC2|CDC0)
    #define CDC_VAL6    (CDC2|CDC1)
    #define CDC_VAL7    (CDC2|CDC1|CDC0)
#define CDC_STANDBY             (CDC_VAL0)  /**< (Comparator Off) Standby Mode */
#define CDC_MSMTONLY            (CDC_VAL1)  /**< Comparator Off;                  Vref not powered down between msmts;    13ms cell voltage msmt time */
#define CDC_13ms                (CDC_VAL2)  /**< 13ms UV/OV comparator period;    Vref not powered down between msmts;    13ms cell voltage msmt time */
#define CDC_130ms               (CDC_VAL3)  /**< 130ms UV/OV comparator period;   Vref not powered down between msmts;    13ms cell voltage msmt time */
#define CDC_500ms               (CDC_VAL4)  /**< 500ms UV/OV comparator period;   Vref not powered down between msmts;    13ms cell voltage msmt time */
#define CDC_130ms_VrefOff       (CDC_VAL5)  /**< 130ms UV/OV comparator period;   Vref powered down between msmts;        21ms cell voltage msmt time */
#define CDC_500ms_VrefOff       (CDC_VAL6)  /**< 500ms UV/OV comparator period;   Vref powered down between msmts;        21ms cell voltage msmt time */
#define CDC_2000ms_VrefOff      (CDC_VAL7)  /**< 2000ms UV/OV comparator period;  Vref powered down between msmts;        21ms cell voltage msmt time */
/** @} */ /* end of group cfgr0 */

/* CFGR1 (Config Register 1) */
/* DCCx = Discharge Cell x; x = 1-12; 0 = turn off shorting switch for cell 'x' (default), 1 = turn on shorting switch */
#define DCC8    (1<<7)
#define DCC7    (1<<6)
#define DCC6    (1<<5)
#define DCC5    (1<<4)
#define DCC4    (1<<3)
#define DCC3    (1<<2)
#define DCC2    (1<<1)
#define DCC1    (1<<0)

/* CFGR2 (Config Register 2) */
/* MCxI = Mask Cell x Interrupts; x = 1-12; 0 = enable interrupts for cell 'x' (default), 1 = turn off interrupts and clear flags for cell 'x' */
#define MC4I    (1<<7)
#define MC3I    (1<<6)
#define MC2I    (1<<5)
#define MC1I    (1<<4)
/* DCCx: see note above in CFGR1 */
#define DCC12   (1<<3)
#define DCC11   (1<<2)
#define DCC10   (1<<1)
#define DCC9    (1<<0)

/* CFGR3 (Config Register 3) */
#define MC12I   (1<<7)
#define MC11I   (1<<6)
#define MC10I   (1<<5)
#define MC9I    (1<<4)
#define MC8I    (1<<3)
#define MC7I    (1<<2)
#define MC6I    (1<<1)
#define MC5I    (1<<0)

/* CFG4 (Config Register 4) */
/* V_UV = Undervoltage Comparison Voltage; Comparison voltage = (V_UV-31)*16*1.5mV */
/* it's better just to cast a uint or numeric literal to a BYTE for this instead of setting these manually */
#define VUV7    (1<<7)
#define VUV6    (1<<6)
#define VUV5    (1<<5)
#define VUV4    (1<<4)
#define VUV3    (1<<3)
#define VUV2    (1<<2)
#define VUV1    (1<<1)
#define VUV0    (1<<0)

/* CFG5 (Config Register 5) */
/* V_OV = Overvoltage Comparison Voltage; Comparison voltage = (V_OV-31)*16*1.5mV */
/* it's better just to cast a uint or numeric literal to a BYTE for this instead of setting these manually */
#define VOV7    (1<<7)
#define VOV6    (1<<6)
#define VOV5    (1<<5)
#define VOV4    (1<<4)
#define VOV3    (1<<3)
#define VOV2    (1<<2)
#define VOV1    (1<<1)
#define VOV0    (1<<0)

union BpsConfig {
    struct {
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
            BYTE data[6];
        };
        BYTE pec;
    };
    BYTE byteArr[7];
};

typedef union {
    struct {
        unsigned val :24;
    } __attribute__((__packed__));
    struct {
        unsigned voltage1 :12;
        unsigned voltage2 :12;
    } __attribute__((__packed__));
    struct {
        BYTE lb;
        BYTE mb;
        BYTE hb;
    };
} RawCellVoltagePair;

union RawVoltages {
    struct {
        union {
            BYTE data[18];
            RawCellVoltagePair voltagePair[6];
        };
        BYTE pec;
    };
    BYTE byteArr[19];
};

union Diagnostic {
    struct {
        union {
            struct {
                BYTE dgnr0;
                BYTE dgnr1;
            };
            struct {
                unsigned ref        :12;
                unsigned unused     :1;
                unsigned muxfail    :1;
                unsigned rev        :2;
            } __attribute__ ((__packed__));
            BYTE data[2];
        };
        BYTE pec;
    };
    BYTE byteArr[3];
};

inline BYTE
calcPEC (const BYTE *in, size_t len) __attribute__((always_inline));

void
BPSInit(SpiChannel chn, const BYTE cfgr0, const BYTE cfgr1, const BYTE cfgr2, const BYTE cfgr3, const BYTE cfgr4, const BYTE cfgr5);

void
BPSInits(SpiChannel chn, unsigned int numDevices, union BpsConfig *cfgs);

void
ReadCfg(SpiChannel chn, unsigned int numDevices, union BpsConfig *cfgs);

void
ReadRawVoltages(SpiChannel chn, unsigned int numDevices, union RawVoltages *cvrs);

void
ConvertVoltages(unsigned int numDevices, union RawVoltages *cvrs, float *cellVoltages);

void
SendCmd(SpiChannel chn, BYTE cmd);

void
ReadDagn(SpiChannel chn, unsigned int numDevices, union Diagnostic *dagn);

void
ConvertDagn(unsigned int numDevices, union Diagnostic *dagn, unsigned int *ref, unsigned int *muxfail, unsigned int *rev);

void
SetVoltageLimits(union BpsConfig *cfgs, double vuv, double vov);

inline UINT32
ConvertToRef(double vref) __attribute__((__always_inline__));

BOOL
ltc6803POSTPassed(union BpsConfig *cfgs);

#endif
