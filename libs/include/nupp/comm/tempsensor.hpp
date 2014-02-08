/*
 * File:   tempsensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 1:03 AM
 */

#ifndef NU_TEMPSENSOR_HPP
#define	NU_TEMPSENSOR_HPP 1

#include "nupp/component/ds18x20.hpp"
#include "nu/config.h"

namespace nu
{
	struct TempSensor
	{
		DS18X20<NU_BPS_N_MODULES> temp_sensor; //on A0
		Array<int16_t, NU_BPS_N_MODULES> temperatures; // in decidegrees C
		Timer temp_measuring_clock;
		bool has_configured_ds;

		TempSensor():
			temp_sensor(PIN(A, 0)),
			temperatures(),
			temp_measuring_clock(NU_BPS_DS18X20_MEASURE_INT_MS, Timer::ms, false),
			has_configured_ds(false)
		{
		}

		void read()
		{
			if (this->temp_measuring_clock.has_expired()) {
				if (this->has_configured_ds)
				{
					this->temp_sensor.update_temperatures(this->temperatures);
				}
				else
				{
					this->has_configured_ds = true;
				}

				this->temp_sensor.perform_temperature_conversion();
				this->temp_measuring_clock.reset();
			}
		}
	};
}

#endif	/* NU_TEMPSENSOR_HPP */

