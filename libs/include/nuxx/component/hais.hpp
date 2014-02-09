/*
 * File:   hais50p.hpp
 * Author: alex
 *
 * Created on June 23, 2013, 2:04 AM
 */

#ifndef HAIS50P_HPP
#define	HAIS50P_HPP

#include "nuxx/component/ad7685.hpp"
#include "nuxx/array.hpp"
#include "nu/compiler.h"

namespace nu
{
	/** Utilities for HAIS current sensor */
	struct HAIS
	{
		/** The Primary Nominal current of the HAIS */
		enum PN
		{
			P50 = 50,
			P100 = 100,
			P150 = 150,
			P200 = 200,
			P400 = 400
		};

		/** Convert ADC voltage output to measured current.
		 * TAKES MICROVOLTS, gives MICROAMPS
		 *
		 * V_out = V_ref + (0.625*I_p/(I_pn))
		 * V_ref = 2.5 +/- 0.025V
		 * I_pn  = primary nominal curent = 50A for HAIS-50P
		 * I_p   = primary current (measured current)
		 *
		 * Ergo, (8*I_pn/5) Amperes/Volt. For example, 80A/V for the HAIS50P.
		 * Max value is technically 0xffffffff --> 343397383600uA ~= 343397A.
		 * Max value is really 4999923 --> 199993840uA ~= 199.99A
		 */
		static PURE INLINE int32_t voltage_to_current(int32_t voltage, PN I_pn)
		{
			return (voltage - 2500000)*(I_pn/5 * 8);
		}
	};
}

#endif	/* HAIS50P_HPP */

