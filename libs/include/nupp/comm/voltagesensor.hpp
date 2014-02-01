/* 
 * File:   voltagesensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 12:16 AM
 */

#ifndef VOLTAGESENSOR_HPP
#define	VOLTAGESENSOR_HPP 1

#include "nupp/component/ltc6803.hpp"
#include "nupp/timer.hpp"

namespace nu
{
	struct VoltageSensor
	{
		LTC6803<3> voltage_sensor; // 3 LTCs
//		Array<float, 32> voltages; // in degC
		Timer voltage_measuring_timer;
		Timer openwire_timer;
		bool has_configured_ltcs;

		INLINE VoltageSensor():
			voltage_sensor(PIN(D, 9), SPI_CHANNEL1),
			voltage_measuring_timer(16, Timer::ms, false),
			openwire_timer(2, Timer::s, false),
			has_configured_ltcs(false)
		{
		}

		void configure()
		{
			/* prepare LTC6804 configuration
			LTC6804<3>::Configuration cfg0 = {0};
			cfg0.bits.adcopt = 0; // normal ADC mode
			cfg0.bits.refon = 1; // stay on
			cfg0.bits.vuv = (voltage_sensor.convert_uv_limit(MIN_VOLTAGE)+1) & 0xfff; // set voltage limit, & mask 12 bits
			cfg0.bits.vov = voltage_sensor.convert_ov_limit(MAX_VOLTAGE) & 0xfff;
			 */
			
			LTC6803<3>::Configuration cfg0;
			memset(cfg0.bytes, 0, sizeof(cfg0.bytes));
			cfg0.bits.cdc = LTC6803<3>::CDC_MSMTONLY;
			cfg0.bits.wdt = true;
			cfg0.bits.lvlpl = true;
			cfg0.bits.vov = voltage_sensor.convert_ov_limit(NU_MAX_VOLTAGE);
			cfg0.bits.vuv = voltage_sensor.convert_uv_limit(NU_MIN_VOLTAGE);
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
					// this->voltage_sensor.read_volts(voltages);
					this->voltage_sensor.update_volts();
				}
				else
				{
					this->has_configured_ltcs = true;
				}

				this->configure();

				// when OpenWire timer expires, switch to measuring Voltage.
				if (this->openwire_timer.has_expired())
				{
					this->voltage_sensor.start_openwire_conversion();
					this->openwire_timer.reset();
				}
				else
				{
					this->voltage_sensor.start_voltage_conversion();
				}

				this->voltage_measuring_timer.reset();
			}
		}
	};
}

#endif	/* VOLTAGESENSOR_HPP */

