/*
 * File:   pin.hpp
 * Author: alex
 *
 * Created on July 6, 2013, 6:33 PM
 */

#ifndef NU_PLATFORM_PIN_HPP
#define	NU_PLATFORM_PIN_HPP

#include "nu/compiler.h"
#include "nu/platform.h"
#include <cstdint>

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nuxx/platform/pin.hpp!"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC /* Generic Pin code */
namespace nu {
	struct PlatformPin {
	};
}
#elif NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific Pin code */
extern "C" {
#include <peripheral/adc10.h>
#include <peripheral/ports.h>
}

#define PIN(L, N) PlatformPin(PlatformPin::L, N) // To specify a pin, use PIN(LETTER, NUMBER)

namespace nu {
	/**
	 * Encapsulate a PIC32 I/O pin. They are addressed by a (letter, number)
	 * combination, represented by an (IoPortId, BIT_X) type combination.
	 */
	struct PlatformPin {
		enum Port {A, B, C, D, E, F, G};

		Port port;
		uint8_t bit;
		static uint16_t enabled_ADCs; // Permits 16 ADC's

		IoPortId ltr() {return (IoPortId)(port);};
		reg_t num() {return (1 << bit);}

		/**
		 * Construct with Pin's (letter, number) combination.
		 * DO NOT CALL THIS CONSTRUCTOR DIRECTLY FROM YOUR CODE!!!
		 * USE THE MACRO PIN(L,N)
		 * @param _port For PIC32MX795F512L, A-G. AnalogIn MUST be B.
		 * @param _bit For PIC32MX795F512L, 0-9.
		 */
		PlatformPin(Port _port = D, uint8_t _bit = 0): port(_port), bit(_bit) {}
		NOINLINE virtual ~PlatformPin() {}

	protected:
		/**
		 * Call one of these four setters from your subclass constructor.
		 */
		virtual void setup_digital_out()	{PORTSetPinsDigitalOut(ltr(), num());}
		virtual void setup_digital_in()	{PORTSetPinsDigitalIn(ltr(), num());}
		virtual void setup_analog_out()	{PORTSetPinsAnalogOut(ltr(), num());}
		virtual void setup_analog_in()		{
			CloseADC10(); // ERROR: Only open 2 channels?
			SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF|ADC_CH0_POS_SAMPLEA_AN0);

			PORTSetPinsAnalogIn(ltr(), num());

			enabled_ADCs |= (typeof(enabled_ADCs)) (1 << bit); // PIC32 SPECIFIC
			uint32_t count_enabled = __builtin_popcount(enabled_ADCs); // 32bit cuz we bitshift below

			/* 1: Turn module on | ouput in integer | trigger mode auto | enable autosample
			 * 2: ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
			 * 3: use ADC internal clock | set sample time
			 * 4: enable all ANX not in use
			 * 5: do not assign other channels to scan */
			#define PARAM1	ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
			#define PARAM2	ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | (count_enabled << _AD1CON2_SMPI_POSITION) | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF // use Channel A only
			#define PARAM3	ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
			#define PARAM4	ENABLE_AN0_ANA | ENABLE_AN1_ANA
			#define PARAM5	SKIP_SCAN_ALL

			OpenADC10( PARAM1, PARAM2, PARAM3, enabled_ADCs, ~enabled_ADCs ); // setup ADC
			EnableADC10();
		}

		virtual void set()		{PORTSetBits(ltr(), num());}
		virtual void clear()		{PORTClearBits(ltr(), num());}
		virtual void toggle()	{PORTToggleBits(ltr(), num());}

		/** A subclass may call EITHER read() OR read_analog(). */
		virtual reg_t read_digital()		{return PORTReadBits(ltr(), num());} // returns 0 or any non-0
		virtual reg_t read_analog(){
			while (!mAD1GetIntFlag()) Nop();
			reg_t val = ReadADC10(bit);
			mAD1ClearIntFlag();
			return val;
		}
	};
}
#endif /* End PLATFORM specific code */

#endif	/* NU_PLATFORM_PIN_HPP */

