/*
 * File:   ds18x20.hpp
 * Author: alex
 *
 * Created on June 24, 2013, 2:30 AM
 */

#ifndef DS18X20_HPP
#define	DS18X20_HPP

#include "nupp/peripheral/onewire.hpp"
#include "nupp/array.hpp"
#include "nu/compiler.h"

namespace nu {
	template <size_t num_devices>
	struct DS18X20: public OneWire {
		static const typename OneWire::romcode roms[32];
		union scratch {
			uint8_t bytes[9];
			struct PACKED {
				int16_t temperature;
				uint8_t T_H;
				uint8_t T_L;
				uint8_t config;
				uint8_t reserved[3];
				uint8_t crc;
			};
		};
		static_assert(sizeof(scratch) == 9, "nu::DS18X20::scratch packing");

		ALWAYSINLINE DS18X20(Pin _p): OneWire(_p) {}

		ALWAYSINLINE void convert_t() {
			this->tx_byte_with_crc( this->CONVERT_T );
		}

		ALWAYSINLINE void perform_temperature_conversion() {
			this->reset();
			this->skip_rom();
			this->convert_t();
		}

		ALWAYSINLINE void read_temperature(const typename OneWire::romcode &code, scratch &scr) {
			this->reset();
			this->match_rom(code);
			this->read_scratch(&scr, sizeof(scr));
		}

		static PURE ALWAYSINLINE float convert_temperature(uint16_t temp) {
			return (float)temp/16;
		}

		ALWAYSINLINE void update_temperatures(Array<float, num_devices> &values) {
			Array<scratch, num_devices> scratches;
			for (unsigned i=0; i<num_devices; i++) {
				this->read_temperature(this->roms[i], scratches[i]);
				values[i] = convert_temperature(scratches[i].temperature);
			}
		}
	};

	template <size_t num_devices>
	const typename OneWire::romcode DS18X20<num_devices>::roms[32] = {
		// bar 1
		{{0x28, 0x9E, 0x63, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x63, 0x58, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xB4, 0x6B, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x77, 0x55, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x73, 0x58, 0xEA, 0x02, 0x00, 0x00}},
		// bar 2
		{{0x28, 0xF3, 0x5F, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xCF, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xAF, 0x67, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x50, 0x74, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xB9, 0x56, 0xEA, 0x02, 0x00, 0x00}},
		// bar 3
		{{0x28, 0x27, 0x61, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x0E, 0x6E, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xE9, 0x2F, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x2E, 0x5B, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x82, 0x5B, 0xEA, 0x02, 0x00, 0x00}},
		// bar 4
		{{0x28, 0xED, 0x65, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x77, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xC3, 0x5A, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x9D, 0x6D, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x1C, 0x3E, 0xEA, 0x02, 0x00, 0x00}},
		// bar 5
		{{0x28, 0x4E, 0x66, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x76, 0x6D, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xF6, 0x73, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xDE, 0x6C, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0x5C, 0x6B, 0xEA, 0x02, 0x00, 0x00}},

		// bar 5.1
		{{0x28, 0x31, 0x62, 0xEA, 0x02, 0x00, 0x00}},
		{{0x28, 0xD0, 0x6B, 0xEA, 0x02, 0x00, 0x00}}
	};
}

#endif	/* DS18X20_HPP */
