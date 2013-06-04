/* 
 * File:   OneWire.hpp
 * Author: alex
 *
 * Created on June 4, 2013, 12:16 AM
 */

#ifndef NUPP_ONEWIRE_HPP
#define	NUPP_ONEWIRE_HPP

#include "nupp/errorcodes.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nu/crc.h"

namespace nu {
	struct OneWire: protected Pin {

		ALWAYSINLINE void high()	{set();}
		ALWAYSINLINE void low()		{clear();}

		ALWAYSINLINE OneWire(Pin _p): Pin(_p) {
			set_digital_out();
			low();
		}
		
		ALWAYSINLINE void power_bus() {
			set_digital_out();
			high();
		}

		ALWAYSINLINE void tx_bit(bool b) {
			low();
			if (b) {
				timer::delay_us<6>();
				high();
				timer::delay_us<64>();
			} else {
				timer::delay_us<60>();
				high();
				timer::delay_us<10>();
			}
		}
		ALWAYSINLINE void tx_byte(uint8_t byte) {
			for (uint8_t i=0; i<8; i++) {
				tx_bit(byte>>i & 1);
			}
		}
		/** @warning check for ENULPTR */
		ALWAYSINLINE void tx(const void *src, size_t n) {
			for (unsigned i=0; i<n; i++) {
				tx_byte(*((uint8_t *)src + i));
			}
		}

		ALWAYSINLINE bool rx_bit() {
			low();
			timer::delay_us<6>();
			high();
			/*
			 * This should theoretically be a 9us delay according to
			 * http://www.maxim-ic.com/app-notes/index.mvp/id/126. However, using
			 * 9us breaks everything. From trial and error, it seems to work using a
			 * delay anywhere from 0us - 7us, so I've chosen to go with the middle
			 * value out of these to avoid potential catastrophe. This should be
			 * somewhere near the middle of the duration for which the ds18x20 is
			 * asserting a bit.
			 */
			timer::delay_us<4>();
			return read();
		}

		ALWAYSINLINE uint8_t rx_byte() {
			uint8_t byte = 0x00;
			for (unsigned j=0; j<8; j++) {
				byte |= (uint8_t)(rx_bit() << j);
			}
			return byte;
		}

		/** @warning pointer */
		ALWAYSINLINE void rx(void *dest, size_t n) {
			uint8_t *bytes = (uint8_t *)dest;
			memset(bytes, 0, n);
			for (unsigned i=0; i<n; i++) {
				bytes[i] = rx_byte();
			}
		}

		ALWAYSINLINE int32_t rx_check_crc(void *dest, size_t n) {
			rx(dest, n);
			uint8_t _crc = rx_byte();
			return (_crc == crc(dest, n))? 0: -error::ECRC;
		}

		ALWAYSINLINE int32_t reset() {
			low();
			timer::delay_us<480>();
			high();
			timer::delay_us<70>();
			bool devices_present = !read();
			timer::delay_us<410>();
			/*high();*/
			return devices_present? 1: -error::ENODEV;
		}



		static const uint32_t crc_table[];
		ALWAYSINLINE uint32_t crc(const void *data, size_t n) {
			if (!data)
				return 0xFFFFFFFF;
			return crcTableFast(crc_table, data, n, 8, CRC_DIRECT, 0x00, CRC_8_DALLAS,
				CRC_REVERSE_DATA_BYTES, CRC_REVERSE_BEFORE_FINAL_XOR, 0x00);
		}
	};
}

#endif	/* NUPP_ONEWIRE_HPP */

