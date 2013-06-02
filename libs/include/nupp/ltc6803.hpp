/* 
 * File:   ltc6803.hpp
 * Author: alex
 *
 * Created on May 31, 2013, 11:12 PM
 */

#ifndef LTC6803_HPP
#define	LTC6803_HPP

#include "nu/compiler.h"
#include "nupp/errorcodes.hpp"
#include "nupp/timer.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/spi.hpp"

namespace nu {
	/** Limited to 32 devices */
	template <uint32_t num_devices>
	struct LTC6803: protected SPI {
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

		union Diagnostic {
			PACKED struct bits {
				unsigned ref		:12;
				unsigned unused		:1;
				unsigned muxfail	:1;
				unsigned rev		:2;
			} bits;
			struct bytes {
				uint8_t b0, b1;
			} bytes;
		};
		union RawVoltagePair {
			unsigned val :24;
			PACKED struct voltages {
				unsigned v1 :12;
				unsigned v2 :12;
			} voltages;
			struct bytes {
				uint8_t b0, b1, b2;
			} bytes;
		};
		union RawVoltages {
			uint8_t bytes[18];
			RawVoltagePair voltage_pair[6];
		};
		union Configuration {
			uint8_t bytes[8];
		};
		template <typename T>
		struct stack {
			T array[num_devices];
		};


		uint32_t mismatch_pecs;
		
		ALWAYSINLINE LTC6803(Pin _cs, SpiChannel _chn):
			SPI(_cs, _chn, 100000, (SpiOpenFlags)(SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON),
			    (SPI::tx_options)(TX_WAIT_START|TX_WAIT_END|TX_DISABLE_AUTO_CS)),
			mismatch_pecs(0) {}


		ALWAYSINLINE void write_configs(stack<Configuration> &config) {
			write_cmd_tx(WRITECFGS, config.array, sizeof(Configuration));
			confirm_configs(config);
		}
		ALWAYSINLINE void start_voltage_conversion() {write_cmd(STCVAD);}
		ALWAYSINLINE void start_openwire_conversion() {write_cmd(STOWAD);}
		ALWAYSINLINE void read_volts(float *voltages) {
			stack<RawVoltages> rx_rv;
			read_volts_raw(rx_rv);
			convert_voltages(rx_rv, voltages);
		}

	private:
		ALWAYSINLINE void confirm_configs(stack<Configuration> &config) {
			stack<Configuration> rx_config;
			read_configs(rx_config);
			// WARNING
		}
		ALWAYSINLINE void read_diags(stack<Diagnostic> &diag) {
			write_cmd(DAGN);
			timer::delay_ms<25>();
			write_cmd_tx(READ_DAGN, diag.array, sizeof(Diagnostic));
		}
		ALWAYSINLINE void check_open_wire(u16 *open_wire, const float *voltages){}
		ALWAYSINLINE void read_configs(stack<Configuration> &config) {write_cmd_rx(RDCFGS, config.array, sizeof(Configuration));}
		ALWAYSINLINE void read_volts_raw(stack<RawVoltages> &rx_rv) {write_cmd_tx(RDCV, rx_rv.array, sizeof(RawVoltages));}

		ALWAYSINLINE uint32_t write_cmd_rx(const command c, void *dest, const size_t one_element){
			cs.low();
			write_cmd(c);

			mismatch_pecs = 0;
			BYTE recv_buffer[one_element + 1]; // +1 byte for pec
			
			for (unsigned i=0; i<num_devices; i++) {
				rx(recv_buffer, one_element);
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
		ALWAYSINLINE void write_cmd_tx(const command c, const void *data, const size_t one_element) {
			cs.low();
			write_cmd(c);
			for (unsigned i=0; i<num_devices; i++) {
				tx_with_pec((const BYTE *)data + i*one_element, one_element); // WARNING pec order
			}
			cs.high();
		}

		ALWAYSINLINE void write_cmd(const command c) {
			cs.low();
			tx_with_pec(&c, 1);
			cs.high();
		}

		ALWAYSINLINE void tx_with_pec(const void *src, const size_t n) {
			long pec = (long)crc_table_fast(src, n); // WARNING type?
			tx(src, n); // WARNING check pointers / tx errors
			tx(&pec, 1);
		}

		/** @todo error reporting */
		ALWAYSINLINE void post() {
			write_cmd(STCVAD_SELFTEST1);
			timer::delay_ms<25>();

			stack<RawVoltages> rv{};
			read_volts_raw(rv);

			stack<Diagnostic> diag{};
			read_diags(diag);
		}

		PURE ALWAYSINLINE BYTE convert_uv_limit(float vuv) {return (BYTE) ((vuv/(16*.0015)) + 31);}
		PURE ALWAYSINLINE BYTE convert_ov_limit(float vov) {return (BYTE) ((vov/(16*.0015)) + 32);}
		PURE ALWAYSINLINE float convert_ref_to_v(float ref) {return (((float)ref - 512) * .0015);}

		PURE ALWAYSINLINE float convert_voltage(uint32_t raw) {return ((float)raw-512)*0.0015;}
		ALWAYSINLINE void convert_voltages(stack<RawVoltages> &rv, float *voltages) {
			unsigned voltage_pairs_per_dev = sizeof(rv.array[0].voltage_pair)/sizeof(rv.array[0].voltage_pair[0]);
			unsigned cells_per_dev = voltage_pairs_per_dev*2;

			for (unsigned i=0; i<num_devices; i++) {
				for (unsigned j=0; j<voltage_pairs_per_dev; j++) {
					RawVoltagePair &pair = rv.array[i].voltage_pair[j];
					voltages[i*cells_per_dev+j*2]	= convert_voltage(pair.voltages.v1);
					voltages[i*cells_per_dev+j*2+1]	= convert_voltage(pair.voltages.v2);
				}
			}
		}

		static const u8 crctab[];
		static long long crc_table_fast (const void *data, size_t len);
	};
}

#endif	/* LTC6803_HPP */

