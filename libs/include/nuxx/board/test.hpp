/* 
 * File:   test.hpp
 * Author: alex
 *
 * Created on February 18, 2014, 8:53 PM
 */

#ifndef TEST_HPP
#define	TEST_HPP

#include "nuxx/board/nu32.hpp"
#include "nuxx/timer.hpp"
#include "nuxx/wdt.hpp"
#include "nu/compiler.h"

namespace nu
{
	struct TestBoard
	{
		Nu32 nu32;
		Timer one_second;

		TestBoard():
			nu32(Nu32::V2011),
			one_second(1, Timer::s, true) {}

		void setup()
		{
			this->nu32.setup();
			this->nu32.led1.off();
			this->nu32.led2.on();
		}

		NORETURN void run_loop()
		{
			while (true)
			{
				if (one_second.has_expired())
				{
					this->nu32.led1.toggle();
					this->nu32.led2.toggle();
					this->one_second.reset();
				}
			}
		}

		void emergency_shutoff()
		{}

		static void NORETURN main(TestBoard *self);
	};
}

#endif	/* TEST_HPP */

