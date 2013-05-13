#ifndef NUPP_PINCTL_HPP
#define NUPP_PINCTL_HPP 1

/**
 * @file
 * Simple pin wrapper
 */

#include "nu/compiler.h"
#include <cstdint>

#include <string>
#include <plib.h>

namespace nu {
	/**
	 * Encapsulate a PIC32 I/O pin. They are addressed by a (letter, number)
	 * combination, represented by an (IoPortId, BIT_X) type combination.
	 */
	struct Pin {
		enum Port {A, B, C, D, E, F, G};

		Port port;
		uint8_t bit;

		IoPortId ltr() {return (IoPortId)(port);}; // PIC32 SPECIFIC
		uint32_t num() {return (1 << bit);} // PIC32 SPECIFIC

		/**
		 * Construct with Pin's (letter, number) combination.
		 */
		ALWAYSINLINE Pin(Port _port = D, uint8_t _bit = 0):
			port(_port), bit(_bit) {}
		NOINLINE virtual ~Pin() {}
		ALWAYSINLINE Pin(const Pin& p): port(p.port), bit(p.bit) {}
		ALWAYSINLINE Pin& operator =(const Pin& p) {port=p.port; bit=p.bit; return *this;}

	protected:
		/**
		 * Call one of these four setters in your subclass.
		 */
		void ALWAYSINLINE set_digital_out()	{PORTSetPinsDigitalOut(ltr(), num());}
		void ALWAYSINLINE set_digital_in()	{PORTSetPinsDigitalIn(ltr(), num());}
		void ALWAYSINLINE set_analog_out()	{PORTSetPinsAnalogOut(ltr(), num());}
		void ALWAYSINLINE set_analog_in()	{PORTSetPinsAnalogIn(ltr(), num());}


		virtual uint32_t ALWAYSINLINE read() {return PORTReadBits(ltr(), num());} // returns 0 or any non-0
		virtual void ALWAYSINLINE set()		{PORTSetBits(ltr(), num());}
		virtual void ALWAYSINLINE clear()	{PORTClearBits(ltr(), num());}
		virtual void ALWAYSINLINE toggle()	{PORTToggleBits(ltr(), num());}
	};


	struct DigitalIn: protected Pin {
		ALWAYSINLINE DigitalIn(Pin p = Pin()): Pin(p) {set_digital_in();}
		uint32_t ALWAYSINLINE read() {return Pin::read();} // returns 0 or any non-0
	};


	class DigitalOut: protected Pin {
		bool _status;

	public:
		ALWAYSINLINE DigitalOut(Pin p = Pin()): Pin(p), _status(false) {set_digital_out();}

		ALWAYSINLINE void high()	{set();}
		ALWAYSINLINE void low()		{clear();}
		ALWAYSINLINE bool status() {return _status;}

		ALWAYSINLINE DigitalOut &operator= (const bool rhs) {
			if (rhs) set();
			else clear();
			return *this;
		}
	};


	/**
	 * An Analog input Pin, wrapped with an ADC.
	 */
	struct AnalogIn: protected Pin {
		static uint16_t enabled_ADCs; // Permits 16 ADC's
		uint8_t adc;

		/**
		 * @param p For PIC32MX795F512L, p.ltr is always IOPORT_B
		 * @param _adc For PIC32MX795F512L, this number == p.bit
		 */
		ALWAYSINLINE AnalogIn(Pin p = Pin()): Pin(p), adc(p.bit) {
			CloseADC10(); // ERROR: Only open 2 channels?
			SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF|ADC_CH0_POS_SAMPLEA_AN0);
			set_analog_in();

			enabled_ADCs |= (typeof(enabled_ADCs)) (1 << adc); // PIC32 SPECIFIC
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

		ALWAYSINLINE uint32_t read(){
			while (!mAD1GetIntFlag()) Nop();
			uint32_t val = ReadADC10(adc);
			mAD1ClearIntFlag();
			return val;
		}
	};
}

#endif
