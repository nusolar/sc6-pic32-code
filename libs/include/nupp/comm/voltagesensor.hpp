/* 
 * File:   voltagesensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 12:16 AM
 */

#ifndef NU_VOLTAGESENSOR_HPP
#define	NU_VOLTAGESENSOR_HPP 1

#include "nupp/component/ltc6803.hpp"
#include "nupp/timer.hpp"

namespace nu
{
	struct VoltageSensor
	{
		static const uint16_t MAX_VOLTAGE = 43000; // multiple of 100uV
		static const uint16_t MIN_VOLTAGE = 27500; // 100uV

		LTC6803<3> voltage_sensor; // 3 LTCs
		Array<uint16_t, 3*12> voltages; // in 100uv. 4 extra voltages, for unused LTC slots
		Timer voltage_measuring_timer;
		Timer openwire_timer;
		bool has_configured_ltcs;

		INLINE VoltageSensor():
			voltage_sensor(PIN(D, 9), SPI_CHANNEL1),
			voltages(0),
			voltage_measuring_timer(16, Timer::ms, false),
			openwire_timer(2, Timer::s, false),
			has_configured_ltcs(false)
		{
		}

		void configure()
		{
			/* LTC6804
			// send random data, to wake LTCs
			voltage_sensor.cs.low();
			voltage_sensor.tx(&state.mode, 1);
			voltage_sensor.cs.high();
			timer::delay_ms(3);

			// prepare LTC6804 configuration
			LTC6804<3>::Configuration cfg0 = {0};
			cfg0.bits.adcopt = 0; // normal ADC mode
			cfg0.bits.refon = 1; // stay on
			cfg0.bits.vuv = (voltage_sensor.convert_uv_limit(MIN_VOLTAGE)+1) & 0xfff; // set voltage limit, & mask 12 bits
			cfg0.bits.vov = voltage_sensor.convert_ov_limit(MAX_VOLTAGE) & 0xfff;

			// write LTC6804 config register, also resetting LTC's WatchDogTimer
			Array<LTC6804<3>::Configuration, 3> cfgs;
			cfgs = cfg0;
			voltage_sensor.write_configs(cfgs);
			 */
			
			LTC6803<3>::Configuration cfg0;
			memset(cfg0.bytes, 0, sizeof(cfg0.bytes));
			cfg0.bits.cdc = LTC6803<3>::CDC_MSMTONLY;
			cfg0.bits.wdt = true;
			cfg0.bits.lvlpl = true;
			cfg0.bits.vov = voltage_sensor.convert_ov_limit(MAX_VOLTAGE);
			cfg0.bits.vuv = voltage_sensor.convert_uv_limit(MIN_VOLTAGE);
			Array<LTC6803<3>::Configuration, 3> cfg;
			cfg = cfg0;

			this->voltage_sensor.write_configs(cfg);
		}

		void read()
		{
			if (this->voltage_measuring_timer.has_expired())
			{
				if (this->has_configured_ltcs)
				{
					this->voltage_sensor.read_volts(this->voltages);
				}
				else
				{
					this->has_configured_ltcs = true;
				}

				// First, reconfigure LTCs, in case they've overflown
				// Then, begin next voltage conversion cycle
				this->configure();
				if (this->openwire_timer.has_expired())
				{
					// when OpenWire timer expires, switch to measuring Voltage.
					this->voltage_sensor.start_openwire_conversion();
					this->openwire_timer.reset();
				}
				else
				{
					this->voltage_sensor.start_voltage_conversion();
				}

				// The timer will expire after the next conversion is complete.
				this->voltage_measuring_timer.reset();
			}
		}
	};
}

#endif	/* NU_VOLTAGESENSOR_HPP */

