/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on July 5, 2013, 7:34 PM
 */

#include "nu/common_pragmas.h"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include <cstdlib>

namespace nu {
	struct USBInterface: public Nu32 {
		Serial ctl;
		static const char *uuid;

		USBInterface(): Nu32(Nu32::V2011), ctl(UART2, 115200) {}

		/**
		 * Called from process_record() with the record's key.
         * @param command The record's key, a null-terminated string.
         */
		virtual void handle_command(char *command) {
			if (strcmp(command, "name") || strcmp(command, "id")) {
				ctl.tx(uuid, strlen(uuid));
			}
		}

		/**
		 * Called when a Record Separator (0x1E) is detected. This function listens
		 * for the command key, or another (premature) record separator.
         * @return 0 if record is handled, >0 if a premature 0x1E is received.
         */
		byte process_record() {
			char command[5];
			size_t i=0;
			for (; i<5; i++) {
				ctl.rx(command + i, 1);

				// if any byte == 0x1E, then command is malformed:
				if (command[i] == TTY_RECORD) return TTY_RECORD;
				// if any byte == 0x1F, then command is complete:
				if (command[i] == TTY_UNIT) break;
			}

			// if last RX'd byte != 0x1F, give up on packet:
			if (command[i] != TTY_UNIT) return 0;

			
			command[i] = '\0'; // convert to null-terminated string
			for (size_t ui=0; ui < i; ui++)
				command[ui] = (char)tolower(command[ui]); // to lowercase
			handle_command(command);
			return 0; // done with packet
		}

		/**
		 * Read a byte. If Record Separator is received, attempt to handle.
         */
		void listen() {
			char first;
			ctl.rx(&first, sizeof(first));
			if (first != TTY_RECORD) return; // error if byte != 0x1E
			while (process_record()) {}
		}
	};

	struct USBDigitalOut: USBInterface {
		DigitalOut relay;
		
		USBDigitalOut(DigitalOut _relay): USBInterface(), relay(_relay) {}

		void handle_command(char *command) {
			if (strcmp(command, "on")) {
				relay.high();
			} else if (strcmp(command, "off")) {
				relay.low();
			} else if (strcmp(command, "togg")) {
				relay.toggle();
			} else if (strcmp(command, "stat")) {
				bool stat = relay.status();
				ctl.tx(&stat, sizeof(stat));
			} else {
				USBInterface::handle_command(command);
			}
		}
	};
}

const char *nu::USBInterface::uuid = __DATE__ " " __TIME__;

using namespace nu;

int main() {
	USBDigitalOut out(DigitalOut(Pin::F, 0));

	while (true) {
		out.listen();
	}

	return 0;
}

