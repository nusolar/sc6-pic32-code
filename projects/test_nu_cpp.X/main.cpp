/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on May 16, 2013, 11:58 PM
 */

#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"

using namespace std;

int main(int argc, char** argv) {
	nu::Nu32 nu32(Nu32::V1);
	while (true) {
		nu32.led1.toggle();
		nu::timer::delay_s<1>();
	}
	return 0;
}

