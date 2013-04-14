//
//  nu32.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/13/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nu32.h"

using namespace nu;



#define X(Type, name, ...) , name(__VA_ARGS__)
Nu32::Nu32(versions _version, uint64_t _hz): version(_version)
	LIST_OF_PINS_V1
#undef  X
{
	nu_hz = _hz;
	
	SYSTEMConfig(HZ, SYS_CFG_ALL);
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	INTEnableSystemMultiVectoredInt();
	DDPCONbits.JTAGEN = 0; // get back A4 and A5
	CVREFOpen(CVREF_DISABLE); // A9 and A10
	/* try to get back A9 and A10  */
#define my_OpenADC10(config1, config2, config3, configport, configscan) (mPORTBSetPinsAnalogIn(configport), AD1CSSL = (unsigned)~(configscan), AD1CON3 = (config3), AD1CON2 = (config2), AD1CON1 = (config1) )
	my_OpenADC10(ADC_MODULE_OFF, ADC_VREF_AVDD_AVSS, 0, 0, 0);
	EthEnable(0); // ethernet
	mPMPDisable(); // parallel port
	
	led1.setup();
	led2.setup();
	switch1.set_digital_in();
}