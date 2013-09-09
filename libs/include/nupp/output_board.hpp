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
#include <cstdint>

#define NU_OUT_TIMEOUT_INT 50 // ms

namespace nu {
	struct OutputBoard {
		bool status;
		uint64_t timeout_clock;
		DigitalOut relay;
		Serial uart;

		OutputBoard(): status(false), timeout_clock(-1),
			relay(PIN(B,0), false), uart(UART(2)) {
		}

		INLINE void recv_serial() {
			// get line
			Array<char, 51> msg;
			size_t length = uart.read_line(msg, msg.size()-1);
			
			// terminate string with NULL character
			msg[length] = '\0';

			if (length > 0 &&
				msg[0] == TTY_UNIT &&
				strncmp(msg.data()+1, "command", 7) == 0 &&
				msg[5] == TTY_RECORD)
			{
				char command[2] = {msg[6], '\0'};
				status = atoi(command);
			} else {
				length = 0;
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
			uart << TTY_UNIT << "status" << TTY_RECORD << status << TTY_UNIT << nu::end;;
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

