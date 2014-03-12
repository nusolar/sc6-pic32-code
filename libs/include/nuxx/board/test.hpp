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
		Nu32 base;
		Timer one_second;

		TestBoard():
			base(Nu32::V2011),
			one_second(1, Timer::s, true) {}

		void setup()
		{
			this->base.setup();
		}

		NORETURN void run_loop()
		{
			this->base.led1.off();
			this->base.led2.on();
			while (true)
			{
				if (one_second.has_expired())
				{
					this->base.led1.toggle();
					this->base.led2.toggle();
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

