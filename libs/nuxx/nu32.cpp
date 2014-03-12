#include "nuxx/board/nu32.hpp"
#include "nuxx/wdt.hpp"
extern "C" {
#include <plib.h>
}

using namespace nu;

void Nu32::PlatformNu32Init(uint32_t _hz) {
	param::Hz() = _hz;

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
}
