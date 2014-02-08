/*
 * File:   currentsensor.hpp
 * Author: alex
 *
 * Created on January 28, 2014, 12:33 AM
 */

#ifndef NU_CURRENTSENSOR_HPP
#define	NU_CURRENTSENSOR_HPP 1

#include "nupp/component/hais.hpp"
#include "nupp/component/ad7685.hpp"

namespace nu
{
	/**
	 * This current sensor obtains readings from the AD7685 with 76.294uV
	 * precision. These represent readings from the HAIS50P with 6103.5uA
	 * nominal precision (precision from datasheet is actually .01*50A==500mA).
	 *
	 * For space, current readings are stored in centiamps.
	 */
	struct CurrentSensor
	{
		static const HAIS::PN pn = HAIS::P50;
		AD7685<2> current_adc; // 2 ADCs
		Array<int16_t, 2> currents; // centiamps

		CurrentSensor():
			current_adc(
				AD7685<2>(PIN(F, 12), SPI_CHANNEL4, PIN(F, 12), // Convert & CS are same pin
				AD7685<2>::CHAIN_MODE_NO_BUSY)),
			currents(0)
		{
		}

		void read()
		{
			// Store 32bit microvoltages, 0-5V, in buffer. Max value 4999923uV
			Array<uint32_t, 2> buffer;
			this->current_adc.convert_read_uv(buffer);
			for (unsigned iCurrent=0; iCurrent<2; iCurrent++)
			{
				// Downcast to 16bit is safe, because maximum value is +- 20000cA
				this->currents[iCurrent] = (int16_t)(HAIS::voltage_to_current(buffer[iCurrent], pn)/10000);
			}
		}
	};
}

#endif	/* NU_CURRENTSENSOR_HPP */

