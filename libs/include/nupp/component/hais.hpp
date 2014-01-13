/* 
 * File:   hais50p.hpp
 * Author: alex
 *
 * Created on June 23, 2013, 2:04 AM
 */

#ifndef HAIS50P_HPP
#define	HAIS50P_HPP

#include "nupp/component/ad7685.hpp"
#include "nupp/array.hpp"
#include "nu/compiler.h"

namespace nu {
	namespace hais {
		enum PN {
			P50 = 50,
			P100 = 100,
			P150 = 150,
			P200 = 200,
			P400 = 400
		};
	}

	/** Utilities for HAIS current sensor */
	template <uint32_t num_devices>
	struct HAIS: public AD7685<num_devices> {
		/** The Primary Nominal current of the HAIS */
		hais::PN I_pn;
		
		Array<float, num_devices> values;
		INLINE float operator[] (size_t index) const {return values[index];}
		INLINE uint32_t count() {return num_devices;}

		INLINE HAIS(AD7685<num_devices> _adc, hais::PN _I_pn): AD7685<num_devices>(_adc), I_pn(_I_pn), values(0.0f) {}
		
		/** Convert ADC voltage output to measured current */
		PURE INLINE float voltage_to_current(float voltage) {
			/* V_out = V_ref + (0.625*I_p/(I_pn))
			 * V_ref = 2.5 +/- 0.025V
			 * I_pn  = primary nominal curent = 50A for HAIS-50P
			 * I_p   = primary current (measured current) */
			return (voltage - 2.5) * (float)I_pn / 0.625;
		}

		INLINE void read_current() {
			this->convert_read_uv();
			for (unsigned i=0; i<num_devices; i++) {
				values[i] = (1-.02)*values[i] + (.02)*(voltage_to_current((float) this->AD7685<num_devices>::operator[](i) / 1000000) + 4);
			}
		}
	};
}

#endif	/* HAIS50P_HPP */

