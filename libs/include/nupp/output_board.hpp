/* 
 * File:   output_board.hpp
 * Author: alex
 *
 * Created on September 6, 2013, 4:32 PM
 */

#ifndef NUPP_OUTPUT_BOARD_HPP
#define	NUPP_OUTPUT_BOARD_HPP

#include "nupp/serial.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/array.hpp"
#include "nu/compiler.h"
#include "nupp/json.hpp"
#include <cstdint>
#include <string>

#define NU_OUT_TIMEOUT_INT 50 // ms

namespace nu {
	struct OutputBoard {
		bool status;
		uint64_t timeout_clock;
		DigitalOut relay;
		Serial input;
		

		Json::FastWriter fw;
		Json::Reader reader;

		OutputBoard(): status(false), timeout_clock(-1),
			relay(PIN(B,0), false), input(UART(2)),
			fw(), reader() {
		}

		INLINE void recv_serial() {
			// get line
			Array<char, 51> msg;
			size_t length = input.read_line(msg, msg.size()-1);
			
			// terminate string with NULL character
			msg[length] = '\0';

			// Try to handle the line
			Json::Value j_command;
			if (length > 0) {
				try {
					// parse JSON
					reader.parse(msg.data(), msg.data() + length, j_command);
					// update status
					status = j_command["command"].asInt();
				} catch (...) {
					length = 0;
				}
			}

			// If handling failed, and if timeout overflowed, reset status.
			// Else if handling succeeded, reset timeout_clock
			if (length == 0) {
				uint64_t time = timer::ms();
				if (time > NU_OUT_TIMEOUT_INT + (time>timeout_clock? timeout_clock: 0)) {
					status = false;
				}
			} else {
				timeout_clock = timer::ms();
			}
		}

		INLINE void send_serial() {
			Json::Value j_report;
			j_report["status"] = status;
			
			std::string report = fw.write(j_report);
			input.puts(report.data());
		}

		INLINE NORETURN void run_loop() {
			while (true) {
				this->recv_serial();
				relay = status;
				this->send_serial();
			}
		}

		INLINE void emergency_shutoff() {
			status = false;
		}

		static NORETURN void main(OutputBoard *out);
	};
}

#endif	/* NUPP_OUTPUT_BOARD_HPP */

