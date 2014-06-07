#ifndef NUXX_PARAM_HPP
#define NUXX_PARAM_HPP 1

#include "nu/compiler.h"
#include "nu/param.h"
#include "nu/platform.h"
#include <cstdint>


#if NU_PLATFORM==NU_PLATFORM_GENERIC /* Generic param code - i.e, nothing*/
namespace nu {
	namespace param {
		static INLINE void PlatformInit() {}
	}
}
#elif NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific Parameter code */
extern "C" {
#include <plib.h>
}
namespace nu {
	namespace param {
		/**
		 * Get Hz specific to Peripherial Bus if one exists. Otherwise get
		 * system clock Hz.
		 * @todo: Distinguish DEFAULT_PBUS_HZ from PBUS_HZ.
		 */
		static INLINE typeof(NU_PBUS_FREQ_HZ) pbus_hz() {return NU_PBUS_FREQ_HZ;}
		static PURE INLINE reg_t timer_ticks_per_cycle() {return 2;}

		static reg_t& Hz();
		static INLINE void PlatformInit() {
			SYSTEMConfig(param::Hz(), SYS_CFG_ALL);
			INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
			INTEnableSystemMultiVectoredInt();
			DDPCONbits.JTAGEN = 0; // get back A4 and A5
			CVREFOpen(CVREF_DISABLE); // A9 and A10
			/* try to get back A9 and A10  */
			#define my_OpenADC10(config1, config2, config3, configport, configscan)\
				(mPORTBSetPinsAnalogIn(configport), AD1CSSL = (unsigned)~(configscan),\
				AD1CON3 = (config3), AD1CON2 = (config2), AD1CON1 = (config1) )
			my_OpenADC10(ADC_MODULE_OFF, ADC_VREF_AVDD_AVSS, 0, 0, 0);
			EthEnable(0); // ethernet
			mPMPDisable(); // parallel port
			// WDT is left in default state

			//OpenCoreTimer(CORE_TICK_RATE);
			//mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
		}
	}
}
#endif /* PLATFORM specific code */


namespace nu {
	/**
	 * System parameters, like the clock frequency and peripheral bus frequency.
	 */
	namespace param {
		static ALWAYSINLINE reg_t& Hz()		{ return nu__Param__Hz; }
		static ALWAYSINLINE reg_t  MHz()	{ return NU_MHZ; }

		static ALWAYSINLINE reg_t  default_hz() { return NU_DEFAULT_HZ; }

		static INLINE reg_t timer_hz()		{ return param::Hz()/param::timer_ticks_per_cycle(); }

		/** CALL BEFORE ANY OTHER SETUP */
		static INLINE void Init(uint32_t _hz = default_hz())
		{
			param::Hz() = _hz;
			PlatformInit();
		}
	}
}

#endif
