/*
 * File:   OneWire.hpp
 * Author: alex
 *
 * Created on June 4, 2013, 12:16 AM
 */

#ifndef NUPP_ONEWIRE_HPP
#define	NUPP_ONEWIRE_HPP

extern "C" {
#include "nu/crc.h"
}
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/errorcodes.hpp"

namespace nu {
	template <size_t num_devices>
	struct OneWire: protected Pin {
		enum command {
			SEARCH_ROM = 0xF0,
			READ_ROM = 0x33,
			MATCH_ROM = 0x55,
			SKIP_ROM = 0xCC,
			ALARM_SEARCH = 0xEC,

			CONVERT_T = 0x44,
			COPY_SCRATCH = 0x48,
			WRITE_SCRATCH = 0x4E,
			READ_SCRATCH = 0xBE,
			RECALL_EE = 0xB8
		};

		union PACKED romcode {
			uint8_t bytes[7];
			struct {
				uint8_t family;
				uint8_t serial[6];
			};
			uint64_t data :56;
		};
		static_assert(sizeof(romcode) == 7, "nu::OneWire::romcode packing");

		struct search_state {
			uint32_t last_discrep_bit, last_family_discrep_bit;
			bool	prev_search_was_last_dev;
			union romcode_crc {
				uint8_t bytes[8];
				struct {
					romcode rc;
					uint8_t crc;
				};
			} romcode_crc;
		} search_state;
		static_assert(sizeof(search_state.romcode_crc) == 8, "nu::OneWire::rcrc packing");

		ALWAYSINLINE void high()	{set();}
		ALWAYSINLINE void low()		{clear();}

		ALWAYSINLINE OneWire(Pin _p): Pin(_p), search_state() {
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

		ALWAYSINLINE void tx_byte_with_crc(uint8_t byte) {
			tx_byte(byte);
			tx_byte(crc(&byte, 1));
		}

		ALWAYSINLINE void tx_with_crc(const void *src, size_t n) {
			uint8_t the_crc = crc(src, sizeof(src));
			tx(src, n);
			tx_byte(the_crc);
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

		ALWAYSINLINE void rx(void *dest, size_t n) {
			uint8_t *bytes = (uint8_t *)dest; // WARNING Pointer
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
			return (devices_present? 1: -error::ENODEV);
		}

		ALWAYSINLINE void reset_search_state() {
			search_state.prev_search_was_last_dev	= false;
			search_state.last_discrep_bit			= 0;
			search_state.last_family_discrep_bit	= 0;
		}

#define OW_NO_DEVS_LEFT_IN_SEARCH   12345

		/** Get the next bit of the victorious device's ROMCODE.
		 * search_direction is 1 if ____
		 * search_direction is 0 if ____
		 */
		ALWAYSINLINE int32_t get_next_bit(const uint32_t id_bit_number, uint32_t &last_zero_discrep_bit) {
			bool id_bit = rx_bit(); // all devices send the bit
			bool id_bit_complement = rx_bit(); // ..then send the bit's complement
			if ((1 == id_bit) && (1 == id_bit_complement)) {
				reset_search_state();
				return OW_NO_DEVS_LEFT_IN_SEARCH;
			}

			bool search_direction = 0;
			if ((0 == id_bit) && (0 == id_bit_complement)) {
				if (id_bit_number == search_state.last_discrep_bit) {
					search_direction = 1;
				} else if (id_bit_number > search_state.last_discrep_bit) {
					search_direction = 0;
				} else { //search direction becomes idBitNumber bit in romCodeAndCrc
					search_direction = search_state.romcode_crc.bytes[(id_bit_number-1)/8] & (1<<((id_bit_number-1)%8));
				}

				if (search_direction == 0) {
					last_zero_discrep_bit = id_bit_number;
					if (last_zero_discrep_bit <= 8) {
						search_state.last_family_discrep_bit = last_zero_discrep_bit;
					}
				}
			} else {
				search_direction = id_bit;
			}
			return search_direction;
		}

		/** The Search ROM algorithm, see www.maxim-ic.com/ibuttonbook */
		ALWAYSINLINE bool search_rom(const romcode &dest, const uint8_t search_rom_cmd) {
			uint32_t id_bit_number = 1; // 1-64
			// bit # of last 0 where there was a discrepency. 1-64, or 0 if none
			uint32_t last_zero_discrep_bit = 0;

			if (!reset()) return false;
			if (search_state.prev_search_was_last_dev) {
				reset_search_state();
				return false;
			}

			tx_byte(search_rom_cmd);
			// Recurse over all bits of a 1Wire serial number
			while (id_bit_number <= 64) {
				int32_t next_bit = get_next_bit(id_bit_number, last_zero_discrep_bit);
				if (next_bit == OW_NO_DEVS_LEFT_IN_SEARCH) {
					reset_search_state();
					return false;
				}
				if (next_bit)
					search_state.romcode_crc.bytes[(id_bit_number-1)/8] |= (uint8_t)(1<<((id_bit_number-1)%8));
				else
					search_state.romcode_crc.bytes[(id_bit_number-1)/8] &= (uint8_t)(~(1<<((id_bit_number-1)%8)));
				tx_bit((bool)next_bit);
				id_bit_number++;
			}
			search_state.last_discrep_bit = last_zero_discrep_bit;
			if (search_state.last_discrep_bit == 0) { // No discrepencies found
				search_state.prev_search_was_last_dev = TRUE;
			}
			uint8_t the_crc = search_state.romcode_crc.crc;
			dest = search_state.romcode_crc.rc;
			if (the_crc != this->crc(&search_state.romcode_crc.rc, sizeof(search_state.romcode_crc.rc)))
				return -1;
			return true;
		}

		ALWAYSINLINE void match_rom(const romcode &code) {
			tx_byte_with_crc(MATCH_ROM);
			tx_with_crc(&code, sizeof(code));
		}

		ALWAYSINLINE void skip_rom() {
			tx_byte_with_crc(SKIP_ROM);
		}

		ALWAYSINLINE void read_scratch(void *dest, size_t n) {
			tx_byte_with_crc(READ_SCRATCH);
			rx(dest, n);
		}


		static const uint32_t crc_table[];
		ALWAYSINLINE uint32_t crc(const void *data, size_t n) {
			if (!data)
				return 0xFFFFFFFF;
			return n;
//			return crcTableFast(crc_table, data, n, 8, CRC_DIRECT, 0x00, CRC_8_DALLAS,
//				CRC_REVERSE_DATA_BYTES, CRC_REVERSE_BEFORE_FINAL_XOR, 0x00);
		}
	};

	template <size_t num_devices>
	const uint32_t OneWire<num_devices>::crc_table[] = {
	   0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65,
	   157,195,33,127,252,162,64,30,95,1,227,189,62,96,130,220,
	   35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98,
	   190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255,
	   70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7,
	   219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154,
	   101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36,
	   248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185,
	   140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205,
	   17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80,
	   175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238,
	   50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115,
	   202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139,
	   87,9,235,181,54,104,138,212,149,203,41,119,244,170,72,22,
	   233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168,
	   116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53
	};
}

#endif	/* NUPP_ONEWIRE_HPP */
