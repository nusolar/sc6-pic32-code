/* 
 * File:   tempsensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 1:03 AM
 */

#ifndef TEMPSENSOR_HPP
#define	TEMPSENSOR_HPP 1

#include "nupp/component/ds18x20.hpp"

namespace nu
{
	struct TempSensor
	{
		DS18X20<32> temp_sensor; //on A0
		Array<float, 32> temperatures; // in degC

		INLINE TempSensor():
			temp_sensor(PIN(A, 0)),
			temperatures()
		{
		}

		void read()
		{
			// NEED TIMER
			this->temp_sensor.update_temperatures(this->temperatures);
			this->temp_sensor.perform_temperature_conversion();
		}
	};
}

#endif	/* TEMPSENSOR_HPP */

