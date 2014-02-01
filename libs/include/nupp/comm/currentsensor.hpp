/* 
 * File:   currentsensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 12:33 AM
 */

#ifndef CURRENTSENSOR_HPP
#define	CURRENTSENSOR_HPP 1

#include "nupp/component/hais.hpp"

namespace nu
{
	struct CurrentSensor
	{
		HAIS<2> current_sensor; // 2 ADCs
		int16_t current0, current1; // 16bit voltage, 0-5V

		INLINE CurrentSensor():
			current_sensor(
				AD7685<2>(PIN(F, 12), SPI_CHANNEL4, PIN(F, 12), // Convert & CS are same pin
				AD7685<2>::CHAIN_MODE_NO_BUSY), hais::P50)
		{
		}

		void read()
		{
			// NEED TIMER
			this->current_sensor.read_current();
		}
	};
}

#endif	/* CURRENTSENSOR_HPP */

