/* 
 * File:   ltc6803.hpp
 * Author: alex
 *
 * Created on May 31, 2013, 11:12 PM
 */

#ifndef LTC6803_HPP
#define	LTC6803_HPP 1

#include "nupp/spi.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/errorcodes.hpp"
#include "nupp/timer.hpp"
#include "nupp/array.hpp"
#include "nu/compiler.h"

namespace nu {
	/** Limited to 32 devices */
	template <uint32_t num_devices>
	struct LTC6803: public SPI {
		enum command {
			WRITECFGS   = 0x01, // Write config registers
			RDCFGS,				// Read config registers
			RDCV        = 0x04, // Read all voltages registers
			RDCVA       = 0x06, // Read voltages 1-4
			RDCVB       = 0x08, //               5-8
			RDCVC       = 0x0A, //               9-12
			RDFLG       = 0x0C, // Read flag registers
			RDTMP       = 0x0E, // Read temp registers
			STCVAD      = 0x10, // Start voltage conversions & poll
			STCVAD_1,
			STCVAD_2,
			STCVAD_3,
			STCVAD_4,
			STCVAD_5,
			STCVAD_6,
			STCVAD_7,
			STCVAD_8,
			STCVAD_9,
			STCVAD_10,
			STCVAD_11,
			STCVAD_12,
			STCVAD_CLR,
			STCVAD_SELFTEST1,
			STCVAD_SELFTEST2,
			STOWAD      = 0x20, // Start open-wire conversions & poll
			STOWAD_1,
			STOWAD_2,
			STOWAD_3,
			STOWAD_4,
			STOWAD_5,
			STOWAD_6,
			STOWAD_7,
			STOWAD_8,
			STOWAD_9,
			STOWAD_10,
			STOWAD_11,
			STOWAD_12,
			STTMPAD     = 0x30, // Start temperature conversions & poll
			STTMPAD_EXT1,
			STTMPAD_EXT2,
			STTMPAD_INTERNAL,
			STTMPAD_SELFTEST1,
			STTMPAD_SELFTEST2,
			POLLADC     = 0x40, // Poll converter status
			POLLINT     = 0x50, // Poll interrupt status
			DAGN        = 0x52, // Start diagnose & poll
			READ_DAGN   = 0x54, // Read diagnostic register
			STCV_DISCHRG    = 0x60, // Start voltage conversions & poll, discharge permitted
			STCV_DISCHRG1,
			STCV_DISCHRG2,
			STCV_DISCHRG3,
			STCV_DISCHRG4,
			STCV_DISCHRG5,
			STCV_DISCHRG6,
			STCV_DISCHRG7,
			STCV_DISCHRG8,
			STCV_DISCHRG9,
			STCV_DISCHRG10,
			STCV_DISCHRG11,
			STCV_DISCHRG12,
			STOW_DISCHRG    = 0x70, // Start open-wire conversions & poll, discharge permitted
			STOW_DISCHRG1,
			STOW_DISCHRG2,
			STOW_DISCHRG3,
			STOW_DISCHRG4,
			STOW_DISCHRG5,
			STOW_DISCHRG6,
			STOW_DISCHRG7,
			STOW_DISCHRG8,
			STOW_DISCHRG9,
			STOW_DISCHRG10,
			STOW_DISCHRG11,
			STOW_DISCHRG12
		};

		enum cfg0 {
			CDC_STANDBY = 0,
			CDC_MSMTONLY,
			CDC_13ms,
			CDC_130ms,
			CDC_500ms,
			CDC_130ms_VrefOff,
			CDC_500ms_VrefOff,
			CDC_2000ms_VrefOff
		};

		union Diagnostic {
			struct PACKED bits {
				unsigned ref		:12;
				unsigned unused		:1;
				unsigned muxfail	:1;
				unsigned rev		:2;
			} bits;
			struct bytes {
				uint8_t b0, b1;
			} bytes;
		};

		static_assert(sizeof(Diagnostic)==2, "nu::LTC6803::Diagnostic packing");
		union Configuration {
			uint8_t bytes[6];
			struct PACKED bits_t {
				unsigned cdc    :3;
				unsigned cell10 :1;
				unsigned lvlpl  :1;
				unsigned gpio1  :1;
				unsigned gpio2  :1;
				unsigned wdt    :1;
				unsigned dcc1   :1;
				unsigned dcc2   :1;
				unsigned dcc3   :1;
				unsigned dcc4   :1;
				unsigned dcc5   :1;
				unsigned dcc6   :1;
				unsigned dcc7   :1;
				unsigned dcc8   :1;
				unsigned dcc9   :1;
				unsigned dcc10  :1;
				unsigned dcc11  :1;
				unsigned dcc12  :1;
				unsigned mc1i   :1;
				unsigned mc2i   :1;
				unsigned mc3i   :1;
				unsigned mc4i   :1;
				unsigned mc5i   :1;
				unsigned mc6i   :1;
				unsigned mc7i   :1;
				unsigned mc8i   :1;
				unsigned mc9i   :1;
				unsigned mc10i  :1;
				unsigned mc11i  :1;
				unsigned mc12i  :1;
				u8 vuv;
				u8 vov;
			} bits;
		};
		static_assert(sizeof(Configuration)==6, "nu::LTC6803::Configuration packing");

		#define voltage_pairs_per_dev 6
		#define cells_per_device (voltage_pairs_per_dev*2)

		union PACKED RawVoltagePair {
			unsigned val :24;
			struct PACKED voltages_t {
				unsigned v1 :12;
				unsigned v2 :12;
			} voltages;
			struct bytes_t {
				uint8_t b0, b1, b2;
			} bytes;
		};
		static_assert(sizeof(RawVoltagePair)==3, "nu::LTC6803::RawVoltagePair packing");

		union RawVoltages {
			RawVoltagePair voltage_pair[voltage_pairs_per_dev];
			uint8_t bytes[voltage_pairs_per_dev*sizeof(RawVoltagePair)];
		};
		static_assert(sizeof(RawVoltages)==3*voltage_pairs_per_dev, "nu::LTC6803::RawVoltages packing");

		uint32_t mismatch_pecs;
		bool is_openwire;
		
		Array<float, num_devices*cells_per_device> values;
		/** @warning NO BOUNDS CHECKING */
		ALWAYSINLINE float operator[] (size_t index) const {return values[index];}
		ALWAYSINLINE uint32_t count() {return num_devices;}

		/**
		 * Construct LTC6803 interface over basic SPI.
         * @param _cs The Chip Select pin for the LTC6803.
         * @param _chn The SPI Channel on which lies the LTC6803.
         */
		ALWAYSINLINE LTC6803(Pin _cs, uint8_t _channel):
			SPI(_cs, Spi(_channel, 100000, SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON),
			    (SPI::tx_options)(TX_WAIT_START|TX_WAIT_END|TX_DISABLE_AUTO_CS)),
			mismatch_pecs(0), is_openwire(false), values(0.0f) {}

		ALWAYSINLINE void write_configs(Array<Configuration, num_devices> &config) {
			write_cmd_tx(WRITECFGS, config, sizeof(Configuration));
			confirm_configs(config);
		}
		ALWAYSINLINE void start_voltage_conversion() {write_cmd_solo(STCVAD); is_openwire=false;}
		ALWAYSINLINE void start_openwire_conversion() {write_cmd_solo(STOWAD); is_openwire=true;}
		ALWAYSINLINE void update_volts() {
			Array<RawVoltages, num_devices> rx_rv;
			read_volts_raw(rx_rv);
			convert_voltages(rx_rv);
		}

		PURE ALWAYSINLINE BYTE convert_uv_limit(float vuv) {return (BYTE) ((vuv/(16*.0015)) + 31);}
		PURE ALWAYSINLINE BYTE convert_ov_limit(float vov) {return (BYTE) ((vov/(16*.0015)) + 32);}
		PURE ALWAYSINLINE float convert_ref_to_v(float ref) {return (((float)ref - 512) * .0015);}
		PURE ALWAYSINLINE float convert_voltage(uint32_t raw) {return ((float)raw-512)*0.0015;}
		
	private:
		ALWAYSINLINE bool confirm_configs(Array<Configuration, num_devices> &config) {
			Array<Configuration, num_devices> rx_config;
			read_configs(rx_config);
			uint32_t result = config.byte_compare(rx_config);
			return result;
		}
		ALWAYSINLINE void read_diags(Array<Diagnostic, num_devices> &diag) {
			write_cmd(DAGN);
			timer::delay_ms<25>();
			write_cmd_tx(READ_DAGN, &diag[0], sizeof(Diagnostic));
		}
		ALWAYSINLINE void check_open_wire(u16 *open_wire, const float *voltages){}
		ALWAYSINLINE void read_configs(Array<Configuration, num_devices> &config) {write_cmd_rx(RDCFGS, config, sizeof(Configuration));}
		ALWAYSINLINE void read_volts_raw(Array<RawVoltages, num_devices> &rx_rv) {write_cmd_rx(RDCV, rx_rv, sizeof(RawVoltages));}

		ALWAYSINLINE void write_cmd_solo(const command c) {
			cs.low();
			write_cmd(c);
			cs.high();
		}

		ALWAYSINLINE uint32_t write_cmd_rx(const command c, void *dest, size_t one_element){
			cs.low();
			write_cmd(c);

			mismatch_pecs = 0;
//			BYTE recv_buffer[one_element + 1]; // +1 byte for pec
			BYTE recv_buffer[100];
			for (unsigned i=0; i<num_devices; i++) {
				rx(recv_buffer, one_element + 1);
				memcpy((BYTE *)dest + i*one_element, recv_buffer, one_element);
				
				BYTE pec = recv_buffer[sizeof(recv_buffer) - 1]; // last byte
				BYTE crc = (BYTE)crc_table_fast(recv_buffer, sizeof(recv_buffer) - 1);
				if (pec != crc) {
					mismatch_pecs |= (uint32_t)(1<<i);
				}
			}
			cs.high();
			return mismatch_pecs? -ECRC: 0; // WARNING should use error_reporting
		}

		/** Deals an array of data of size==one_element to all devices */
		ALWAYSINLINE void write_cmd_tx(const command c, const void *data, size_t one_element) {
			cs.low();
			write_cmd(c);
			for (unsigned i=0; i<num_devices; i++) {
				tx_with_pec((const BYTE *)data + i*one_element, one_element); // WARNING pec order
			}
			cs.high();
		}

		ALWAYSINLINE void write_cmd(const command c) {
			tx_with_pec(&c, 1);
		}

		ALWAYSINLINE void tx_with_pec(const void *src, size_t n) {
			long pec = (long)crc_table_fast(src, n); // WARNING type?
			tx(src, n); // WARNING check pointers / tx errors
			tx(&pec, 1);
		}

		/** @todo error reporting */
		ALWAYSINLINE void post() {
			write_cmd(STCVAD_SELFTEST1);
			timer::delay_ms<25>();

			Array<RawVoltages, num_devices> rv();
			read_volts_raw(rv);

			Array<Diagnostic, num_devices> diag();
			read_diags(diag);
		}

		ALWAYSINLINE void convert_voltages(Array<RawVoltages, num_devices> &rv) {
			for (unsigned i=0; i<num_devices; i++) {
				for (unsigned j=0; j<voltage_pairs_per_dev; j++) {
					RawVoltagePair &pair = rv[i].voltage_pair[j];
					(values[i*cells_per_device+j*2]	 += convert_voltage(pair.voltages.v1))/=2;
					(values[i*cells_per_device+j*2+1] += convert_voltage(pair.voltages.v2))/=2;
				}
			}
		}

		static const u8 crctab[];
		static long long crc_table_fast (const void *data, size_t len);
	};

	/**
	 * Based on code found at http://zorc.breitbandkatze.de/crc.html (scroll down on page)
	 *
	 * From LTC6803 data sheet:
	 * characteristic polynomial x^8 + x^2 + x + 1
	 * initial PEC value of 01000001 (0x41)
	 *
	 * Therefore, CRC calculator settings are as follows:
	 * CRC order (1..64)    : 8
	 * CRC polynom (hex)    : 07
	 * Initial value (hex)  : 41 (direct)
	 * Final XOR value (hex): 00
	 * DON'T reverse data bytes
	 * DON'T reverse CRC result before Final XOR
	 */
	template <uint32_t num_devices>
	long long LTC6803<num_devices>::crc_table_fast (const void *data, size_t len) {
		if (unlikely(data == NULL))
			return -ENULPTR;

		const BYTE *dataBytes = (const BYTE *)data;

		unsigned long crc = 0x41;

		while (len--)
			crc = (crc << 8) ^ crctab[ (crc & 0xff) ^ *dataBytes++];
																   /* 8 here is referring to the polynomial order */
		/* static const unsigned long crcmask = ((((unsigned long)1<<(8-1))-1)<<1)|1; */
		unsigned long crcmask = 0xff;

		crc &= crcmask;

		return crc;
	}
	template <uint32_t num_devices>
	const uint8_t LTC6803<num_devices>::crctab[] =
	   {0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
		0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
		0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
		0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
		0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
		0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
		0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
		0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
		0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
		0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
		0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
		0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
		0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
		0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
		0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
		0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
		0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
		0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
		0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
		0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
		0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
		0xFA, 0xFD, 0xF4, 0xF3};
}

#endif	/* LTC6803_HPP */

