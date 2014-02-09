/*
 * File:   ltc6803.hpp
 * Author: alex
 *
 * Created on May 31, 2013, 11:12 PM
 */

#ifndef LTC6803_HPP
#define	LTC6803_HPP 1

#include "nuxx/peripheral/spi.hpp"
#include "nuxx/peripheral/pinctl.hpp"
#include "nuxx/errorcodes.hpp"
#include "nuxx/timer.hpp"
#include "nuxx/crc.hpp"
#include "nuxx/array.hpp"
#include "nu/compiler.h"

namespace nu {
	/**
	 * Limited to 32 devices
	 * LTC6803 uses the CRC-8-CCITT algorithm.
	 *
	 * The LTC6803 outputs its voltages in 12bit UInt's, as multiples of 1.5mV,
	 *     OFFSET BY 0x200 (768mV).
	 * A reading of 0x200 indicates a measurement of 0V == (0x200-0x200)*1.5mV.
	 * Similarly, 0x000 indicates a measurement of -768mV == (0x0-0x200)*1.5mV.
	 * Similarly, 0x800 indicates a measurement of 2.304V == (0x800-0x200)*1.5mV.
	 *
	 * HOWEVER, like the class LTC6804, class LTC6803 receives and returns voltages
	 * in multiples of 100uV, as UInt16's.
	 *
	 * Call read_volts() to synchronously poll & get readings.
	 */
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
		#define total_cells (cells_per_device * num_devices)

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

		/**
		 * Construct LTC6803 interface over basic SPI.
         * @param _cs The Chip Select pin for the LTC6803.
         * @param _chn The SPI Channel for the LTC6803.
         */
		LTC6803(Pin _cs, uint8_t _channel):
			SPI(_cs, Spi(_channel, 100000, SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON),
			    (SPI::tx_options)(TX_WAIT_START|TX_WAIT_END|TX_DISABLE_AUTO_CS)),
			mismatch_pecs(0), is_openwire(false) {}

		void write_configs(Array<Configuration, num_devices> &config) {
			write_cmd_tx(WRITECFGS, config, sizeof(Configuration));
			confirm_configs(config);
		}
		void start_voltage_conversion() {write_cmd_solo(STCVAD); is_openwire=false;}
		void start_openwire_conversion() {write_cmd_solo(STOWAD); is_openwire=true;}
		/* Outputs in 100uV multiples */
		void read_volts(Array<uint16_t, total_cells> &rx_rv) {
			Array<RawVoltages, num_devices> rv;
			read_volts_raw(rv);

			for (unsigned iDevice=0; iDevice<num_devices; iDevice++) {
				for (unsigned iPair=0; iPair<voltage_pairs_per_dev; iPair++) {
					RawVoltagePair &pair = rv[iDevice].voltage_pair[iPair];
#warning "Averaging is for crude low-pass filtering"
					rx_rv[iDevice*cells_per_device+iPair*2]	=
							(uint16_t)(pair.voltages.v1/2 + rx_rv[iDevice*cells_per_device+iPair*2]/2);
					rx_rv[iDevice*cells_per_device+iPair*2+1] =
							(uint16_t)(pair.voltages.v2/2 + rx_rv[iDevice*cells_per_device+iPair*2+1]/2);
				}
			}
		}

		/* Inputs and Outputs in 100uV multiples. Equations from Datasheet p24-p25 */
		PURE BYTE convert_uv_limit(float vuv) {return (BYTE) ((vuv/(16*15)) + 31);}
		PURE BYTE convert_ov_limit(float vov) {return (BYTE) ((vov/(16*15)) + 32);}
		PURE uint16_t convert_ref_to_v(float ref) {return (((float)ref - 0x200) * 15);}
		PURE uint16_t convert_voltage(uint16_t raw) {return (raw - 0x200)*15;}

	private:
		bool confirm_configs(Array<Configuration, num_devices> &config) {
			Array<Configuration, num_devices> rx_config;
			read_configs(rx_config);
			uint32_t result = config.byte_compare(rx_config);
			return result;
		}
		void read_diags(Array<Diagnostic, num_devices> &diag) {
			write_cmd(DAGN);
			timer::delay_ms(25);
			write_cmd_tx(READ_DAGN, &diag[0], sizeof(Diagnostic));
		}
		void check_open_wire(u16 *open_wire, const float *voltages){}
		void read_configs(Array<Configuration, num_devices> &config) {write_cmd_rx(RDCFGS, config, sizeof(Configuration));}
		void read_volts_raw(Array<RawVoltages, num_devices> &rx_rv) {write_cmd_rx(RDCV, rx_rv, sizeof(RawVoltages));}

		void write_cmd_solo(const command c) {
			cs.low();
			write_cmd(c);
			cs.high();
		}

		uint32_t write_cmd_rx(const command c, void *dest, size_t one_element){
			cs.low();
			write_cmd(c);

			mismatch_pecs = 0;
//			BYTE recv_buffer[one_element + 1]; // +1 byte for pec
			BYTE recv_buffer[100];
			for (unsigned i=0; i<num_devices; i++) {
				rx(recv_buffer, one_element + 1);
				memcpy((BYTE *)dest + i*one_element, recv_buffer, one_element);

				BYTE pec = recv_buffer[sizeof(recv_buffer) - 1]; // last byte
				BYTE CRC = (BYTE)crc::_8_ccitt(recv_buffer, sizeof(recv_buffer) - 1);
				if (pec != CRC) {
					mismatch_pecs |= (uint32_t)(1<<i);
				}
			}
			cs.high();
			return mismatch_pecs? -ECRC: 0; // WARNING should use error_reporting
		}

		/** Deals an array of data of size==one_element to all devices */
		void write_cmd_tx(const command c, const void *data, size_t one_element) {
			cs.low();
			write_cmd(c);
			for (unsigned i=0; i<num_devices; i++) {
				tx_with_pec((const BYTE *)data + i*one_element, one_element); // WARNING pec order
			}
			cs.high();
		}

		void write_cmd(const command c) {
			tx_with_pec(&c, 1);
		}

		void tx_with_pec(const void *src, size_t n) {
			long pec = (long)crc::_8_ccitt(src, n); // WARNING type?
			tx(src, n); // WARNING check pointers / tx errors
			tx(&pec, 1);
		}

		/** @todo error reporting */
		void post() {
			write_cmd(STCVAD_SELFTEST1);
			timer::delay_ms(25);

			Array<RawVoltages, num_devices> rv();
			read_volts_raw(rv);

			Array<Diagnostic, num_devices> diag();
			read_diags(diag);
		}
	};
}

#undef voltage_pairs_per_dev
#undef cells_per_device

#endif	/* LTC6803_HPP */

