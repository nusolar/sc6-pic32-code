/* 
 * File:   config.h
 * Author: alex
 *
 * Created on February 4, 2014, 2:10 AM
 */

#ifndef NU_CONFIG_H
#define	NU_CONFIG_H

#ifdef	__cplusplus
extern "C"
{
#endif

#define NU_BPS_N_MODULES	32		// number of battery modules!
#define NU_BPS_MAX_VOLTAGE	43000	// multiple of 100uV
#define NU_BPS_MIN_VOLTAGE	27500	// 100uV
#define NU_BPS_MAX_TEMP		450		// deciCelcius
#define NU_BPS_MIN_TEMP		0
#define NU_BPS_MAX_BATT_CURRENT_DISCHARGING	7200	// centiamps, 72.8
#define NU_BPS_MAX_BATT_CURRENT_CHARGING	(-3200)	// centiamps, -36.4
#define NU_BPS_MAX_CHARGE_VOLTAGE		(NU_BPS_MAX_VOLTAGE + 100)
#define NU_BPS_MIN_DISCHARGE_VOLTAGE	(NU_BPS_MIN_VOLTAGE + 500)

#define NU_BPS_OS_TIMEOUT_INT_MS	100		// ms - timeout for OS command
#define NU_BPS_PRECHARGE_TIME_MS	2000	// ms - duration of motor controller precharge
#define NU_BPS_CONVERSION_TIME_MS	2.3		// ms - time for LTC6804s to complete conversion. UNUSED
#define NU_BPS_LTC6803_OPENWIRE_INT_S	2	// s - time between openwire checks
#define NU_BPS_LTC6803_MEASURE_INT_MS	16	// ms
#define NU_BPS_DS18X20_MEASURE_INT_MS	150	// ms
#define NU_BPS_CAN_TIMER_MS				20	// ms - 32 modules + 3 other ~= 700ms cycles

#define NU_PEDALS_WS20_TIMEOUT_MS	50	// ms - Tritium recommends 50-100ms. MUST BE <250ms or WS20 will shut down.
#define NU_PEDALS_OS_TIMEOUT_MS		100	// ms
#define NU_PEDALS_REGEN_PROPORTION	0.2
#define NU_PEDALS_ACCEL_SENSITIVITY	0.05 // Must be non-zero or high-current tripping happens

#ifdef	__cplusplus
}
#endif

#endif	/* NU_CONFIG_H */
