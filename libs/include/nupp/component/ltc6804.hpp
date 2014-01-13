/* 
 * File:   ltc6804.hpp
 * Author: alex
 *
 * Created on September 8, 2013, 4:18 AM
 */

#ifndef NUPP_LTC6804_HPP
#define	NUPP_LTC6804_HPP 1

#include "nupp/peripheral/spi.hpp"
#include "nupp/array.hpp"
#include "nupp/crc.hpp"
#include "nu/compiler.h"

namespace nu {
	/**
	 * A stack of LTC6804 voltage sensors.
	 *
	 * ---------Under the hood---------
	 * To communicate with the LTC, 2 Command bytes are written over SPI. If the
	 * command implies a Register Group, then 6 data bytes and 2 PEC bytes are
	 * read/written.
	 * All register groups are 6 bytes, and Cell Voltages are stored in UInt16's.
	 * Therefore, one Cell Voltage register group holds 3 voltages.
	 *
	 * Unlike the LTC6803, all Commands and PECs are 2 bytes. Beware, the LTC
	 * sends and receives all Commands and PECs ***MOST SIGNIFICANT BYTE & BIT FIRST***.
	 * However, all data are send LEAST SIGNIFICANT BYTE FIRST. See datasheet for details.
	 *
	 * Note, all SPI transmissions are Most Significant **Bit** first, but that's
	 * handled by the PIC32.
	 *
	 * LTC6804 uses the CRC-15-CAN algorithm.
	 *
	 * The Cell Voltages are reported in multiples of 100uV, as UInt16's.
	 * A reading of 0x80E8 (33,000 decimal) indicates a measurement of 3.3V.
	 */
	template <uint32_t N_DEVICES>
	struct LTC6804: public SPI {
		/**
		 * These flags are for ADC commands (listed below).
		 * They should be bitwise-OR'd with the ADC commands, as needed.
		 */
		enum flags {
			// ADC Mode
			MD_1	= 0x0001 << 7,	// 27kHz, or 14kHz if ADCOPT==1
			MD_2	= 0x0002 << 7,		// 7kHz, or 3kHz if ADCOPT==1
			MD_3	= 0x0003 << 7,	// 26kHz, or 2kHz if ADCOPT==1

			// Discharge permitted?
			DCP_0	= 0x0000 << 4,	// Discharge not permitted
			DCP_1	= 0x0001 << 4,	// Discharge permitted

			// Cell selection
			CH_0	= 0x0000,	// All cells
			CH_1	= 0x0001,	// Cells 1&7
			CH_2	= 0x0002,	//       2&8
			CH_3	= 0x0003,	//       3&9
			CH_4	= 0x0004,	//       4&10
			CH_5	= 0x0005,	//       5&11
			CH_6	= 0x0006,	//       6&12

			// Pull up/down current for Open-Wire
			PUP_0	= 0x0000 << 6,	// Pull-Down Current
			PUP_1	= 0x0001 << 6	// Pull-Up Current

			// etc...
		};

		/**
		 * UInt16 commands. At transmission, these commands are byte-swapped
		 * and sent MSByte, MSBit first.
		 */
		enum command {
			// These commands Write or Read 6 bytes per LTC6804.
			WRCFG	= 0x0001,	// write config register group
			RDCFG	= 0x0002,	// read config register group
			RDCVA	= 0x0004,	// read voltages 1-3
			RDCVB	= 0x0006,	//               4-6
			RDCVC	= 0x0008,	//               7-9
			RDCVD	= 0x000A,	//               10-12
			RDAUXA	= 0x000C,	// read aux 1-3
			RDAUXB	= 0x000E,	//          4-5, and REF bytes
			RDSTATA	= 0x0010,	// read status register group A
			RDSTATB = 0x0012,	//                      group B

			// These ADC commands require flags (listed above).
			ADCV	= 0x0260,	// start Cell Voltage ADC conversion
			ADOW	= 0x0228,	// start Open Wire ADC conversion

			CLRCELL	= 0x0711,	// clear Cell Voltage register groups
			CLRAUX	= 0x0712,	// clear Aux register groups
			CLRSTAT	= 0x0713	// clear Status register groups
			// etc...
		};

		union Configuration {
			unsigned long long value :(8*6) PACKED;
			uint8_t bytes[6];
			struct PACKED bits_t {
				unsigned adcopt	:1;
				unsigned swtrd	:1;
				unsigned refon	:1;
				unsigned gpio1  :1;
				unsigned gpio2  :1;
				unsigned gpio3  :1;
				unsigned gpio4  :1;
				unsigned gpio5  :1;
				unsigned vuv	:12;
				unsigned vov	:12;
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
				unsigned dcto	:4;
			} bits;
		};
		static_assert(sizeof(Configuration)==6, "nu::LTC6804::Configuration packing");

		union PACKED VoltageRegisterGroup {
			unsigned long value :(8*3) PACKED;
			uint16_t voltages[3];
			struct PACKED {
				uint16_t _1;
				uint16_t _2;
				uint16_t _3;
			} r;
		};
		static_assert(sizeof(VoltageRegisterGroup)==6, "nu::LTC6804::VoltageRegisterGroup packing");

		#define voltages_per_reg (3)
		#define regs_per_device (4)
		#define cells_per_device (voltages_per_reg * regs_per_device)
		#define total_cells (cells_per_device * N_DEVICES)

		uint32_t mismatch_pecs;
		bool is_openwire;

		ALWAYSINLINE LTC6804(Pin _cs, uint8_t _channel):
			SPI(_cs, Spi(_channel, 100000, SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON),
			    (SPI::tx_options)(TX_WAIT_START|TX_WAIT_END|TX_DISABLE_AUTO_CS)),
			mismatch_pecs(0), is_openwire(false) {}

		/* LTC Routines */

		ALWAYSINLINE void write_configs(Array<Configuration, N_DEVICES> &config) {
			write_cmd_tx(WRCFG, config, sizeof(Configuration));
			confirm_configs(config);
		}

		ALWAYSINLINE void start_voltage_conversion() {
			write_cmd_solo((command)(ADCV|MD_2|DCP_1));
		}

		ALWAYSINLINE void read_volts(Array<uint16_t, total_cells> &rx_rv) {
			Array<VoltageRegisterGroup, N_DEVICES> vr[4];

			write_cmd_rx(RDCVA, vr[0], sizeof(VoltageRegisterGroup));
			write_cmd_rx(RDCVB, vr[1], sizeof(VoltageRegisterGroup));
			write_cmd_rx(RDCVC, vr[2], sizeof(VoltageRegisterGroup));
			write_cmd_rx(RDCVD, vr[3], sizeof(VoltageRegisterGroup));

			for (unsigned iDevice=0; iDevice<N_DEVICES; ++iDevice) {
				for (unsigned iRegister=0; iRegister<regs_per_device; ++iRegister) {
					for (unsigned iValue=0; iValue<voltages_per_reg; ++iValue) {
						// Register first! then Device (from outer loop). then Value
						rx_rv[iDevice*cells_per_device + iRegister*voltages_per_reg + iValue] = vr[iRegister][iDevice].voltages[iValue];
					}
				}
			}
		}

		/** Input in multiple of 100 microVolts !! */
		PURE ALWAYSINLINE uint16_t convert_uv_limit(uint32_t mV) {return (uint16_t) (mV/16 - 1);}
		PURE ALWAYSINLINE uint16_t convert_ov_limit(uint32_t mV) {return (uint16_t) (mV/16);}

	private:
		/* LTC6804 low-level COMMANDS */

		ALWAYSINLINE bool confirm_configs(Array<Configuration, N_DEVICES> &config) {
			Array<Configuration, N_DEVICES> rx_config;
			read_configs(rx_config);
			bool result = config.byte_compare(rx_config);
			return result;
		}
		
		ALWAYSINLINE void read_configs(Array<Configuration, N_DEVICES> &config) {
			write_cmd_rx(RDCFG, config, sizeof(Configuration));
		}

		/* LTC6804 COMMUNICATION PROTOCOL */

		ALWAYSINLINE void write_cmd_solo(const command c) {
			cs.low();
			write_cmd(c);
			cs.high();
		}

		ALWAYSINLINE uint32_t write_cmd_rx(const command c, void *dest, size_t one_element) {
			cs.low();
			write_cmd(c);

			mismatch_pecs = 0;
//			BYTE recv_buffer[one_element + 2]; // +2 bytes for pec
			BYTE recv_buffer[100];
			
			for (unsigned i=0; i<N_DEVICES; i++) {
				// RX 8 bytes: 6 data + 2 PEC
				rx(recv_buffer, one_element + 2);

				// Copy data into array
				memcpy((BYTE *)dest + i*one_element, recv_buffer, one_element);

				// copy PEC bytes, byte-swapping
				uint16_t pec = (uint16_t)((recv_buffer[one_element]<<8) | recv_buffer[one_element+1]);

				// check PEC
				uint16_t crc = (uint16_t)crc::_15_can(recv_buffer, one_element);
				if (pec != crc) {
					mismatch_pecs |= (uint32_t)(1<<i);
				}
			}
			cs.high();
			return mismatch_pecs? -ECRC: 0; // WARNING should use error_reporting
		}

		/**
		 * Deals an array of data of size==one_element to all devices
		 */
		ALWAYSINLINE void write_cmd_tx(const command c, const void *data, size_t one_element) {
			cs.low();
			write_cmd(c);
			for (unsigned i=0; i<N_DEVICES; i++) {
				tx_with_pec((const BYTE *)data + i*one_element, one_element); // WARNING pec order
			}
			cs.high();
		}

		/**
		 * Compute PEC, byteswap command, and TX 4 bytes.
         * @param c the command
         */
		ALWAYSINLINE void write_cmd(const command c) {
			uint16_t cmd = (uint16_t)c;
			uint16_t swapped_cmd = (uint16_t)(((0xFF & cmd) << 8) | ((0xFF00 & cmd) >> 8));
			tx_with_pec(&swapped_cmd, 2);
		}

		ALWAYSINLINE void tx_with_pec(const void *src, size_t n) {
			uint16_t pec = (uint16_t)crc::_15_can(src, n);
			tx(src, n); // WARNING check pointers / tx errors
			tx(&pec, 2);
		}
	};
}

#undef cells_per_device
#undef total_cells

#endif	/* NUPP_LTC6804_HPP */

