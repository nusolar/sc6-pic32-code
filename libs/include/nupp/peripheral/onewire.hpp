#ifndef ONEWIRE_HPP
#define	ONEWIRE_HPP 1

#include "nupp/peripheral/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/errorcodes.hpp"
#include "nu/crc.h"
#include "nu/compiler.h"


namespace nu {
	struct OneWire: protected AbstractPin {
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

		void high()	{set();}
		void low()		{clear();}

		OneWire(Pin _p): AbstractPin(_p), search_state() {
			set_digital_out();
			low();
		}

		void power_bus() {
			set_digital_out();
			high();
		}

		void tx_bit(bool b) {
			low();
			if (b) {
				timer::delay_us(6);
				high();
				timer::delay_us(64);
			} else {
				timer::delay_us(60);
				high();
				timer::delay_us(10);
			}
		}
		void tx_byte(uint8_t byte) {
			for (uint8_t i=0; i<8; i++) {
				tx_bit(byte>>i & 1);
			}
		}
		/** @warning check for ENULPTR */
		void tx(const void *src, size_t n) {
			for (unsigned i=0; i<n; i++) {
				tx_byte(*((uint8_t *)src + i));
			}
		}

		void tx_byte_with_crc(uint8_t byte) {
			tx_byte(byte);
			tx_byte(crc(&byte, 1));
		}

		void tx_with_crc(const void *src, size_t n) {
			uint8_t the_crc = crc(src, sizeof(src));
			tx(src, n);
			tx_byte(the_crc);
		}

		bool rx_bit() {
			low();
			timer::delay_us(6);
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
			timer::delay_us(4);
			return read_digital();
		}

		uint8_t rx_byte() {
			uint8_t byte = 0x00;
			for (unsigned j=0; j<8; j++) {
				byte |= (uint8_t)(rx_bit() << j);
			}
			return byte;
		}

		void rx(void *dest, size_t n) {
			uint8_t *bytes = (uint8_t *)dest; // WARNING Pointer
			memset(bytes, 0, n);
			for (unsigned i=0; i<n; i++) {
				bytes[i] = rx_byte();
			}
		}

		int32_t rx_check_crc(void *dest, size_t n) {
			rx(dest, n);
			uint8_t _crc = rx_byte();
			return (_crc == crc(dest, n))? 0: -error::ECRC;
		}

		int32_t reset() {
			low();
			timer::delay_us(480);
			high();
			timer::delay_us(70);
			bool devices_present = !read_digital();
			timer::delay_us(410);
			/*high();*/
			return (devices_present? 1: -error::ENODEV);
		}

		void reset_search_state() {
			search_state.prev_search_was_last_dev	= false;
			search_state.last_discrep_bit			= 0;
			search_state.last_family_discrep_bit	= 0;
		}

#define OW_NO_DEVS_LEFT_IN_SEARCH   12345

		/** Get the next bit of the victorious device's ROMCODE.
		 * search_direction is 1 if ____
		 * search_direction is 0 if ____
		 */
		int32_t get_next_bit(const uint32_t id_bit_number, uint32_t &last_zero_discrep_bit) {
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
		bool search_rom(romcode &dest, const uint8_t search_rom_cmd) {
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

		void match_rom(const romcode &code) {
			tx_byte_with_crc(MATCH_ROM);
			tx_with_crc(&code, sizeof(code));
		}

		void skip_rom() {
			tx_byte_with_crc(SKIP_ROM);
		}

		void read_scratch(void *dest, size_t n) {
			tx_byte_with_crc(READ_SCRATCH);
			rx(dest, n);
		}


		static const uint32_t crc_table[];
		uint32_t crc(const void *data, size_t n) {
			if (!data)
				return 0xFFFFFFFF;
			return n;
			// TODO OneWire CRC
//			return nu__CRC__table_fast(crc_table, data, n, 8, CRC_DIRECT, 0x00, CRC_8_DALLAS,
//				CRC_REVERSE_DATA_BYTES, CRC_REVERSE_BEFORE_FINAL_XOR, 0x00);
		}
	};
}

#endif	/* ONEWIRE_HPP */

