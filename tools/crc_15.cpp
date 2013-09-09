#include <stdint.h>
#include <iostream>
#include <sstream>

static uint32_t crc_fast (const void *v_data, size_t len) {
	const uint8_t *data = (uint8_t *)v_data;
	uint16_t crc = 0x10;

	if (len) do {
		// Shift in the next byte, get a new CRC value.

		uint16_t eor;
		unsigned int i = 8;

		crc ^= (uint16_t)(*data++) << 7;
		do {
			/* This might produce branchless code */
			eor = crc & 0x4000 ? 0x4599 : 0;
			crc <<= 1;
			crc^= eor;
		} while (--i);
	} while (--len);

	return (crc & 0x7fff)<<1;
}

int main(int argc, const char *argv[])
{
	uint16_t test = 0x0100;

	if (argc > 1)
	{
		std::stringstream ss;
		ss << argv[1];
		ss >> test;
	}

	uint16_t crc =  crc_fast(&test, 2);

	std::cout << crc << std::endl;

	return 0;
}
