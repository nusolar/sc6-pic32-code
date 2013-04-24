#include "nupp/ad7685.hpp"
#include "nupp/timer.hpp"
#include <alloca.h>
#include <sys/endian.h>

using namespace nu;

void AD7685::read_uv(uint32_t *dst){
	uint16_t *buffer = (uint16_t *)alloca(sizeof(*buffer) * num_devices);
	
	rx(buffer, sizeof(*buffer)*num_devices);
	for (unsigned int ui=0; ui < num_devices; ui++) {
		// swap byte order
        // buf[ui] = bswap_u16(buf[ui]);
        buffer[ui] = betoh16(buffer[ui]);
        // then compute the actual voltage (microvolts)
        dst[ui] = (uint32_t)((5000000 * (uint64_t)buffer[ui])>>16);
	}
}

void AD7685::convert_read_uv(uint32_t *dst){
	if (FOUR_WIRE & opt && BUSY_INDICATOR & opt)
        Pin::high();
	
    // start conversion
    convert.high();
    delay_ns(100);  // .1 us
	
    if (BUSY_INDICATOR & opt) {
        if (THREE_WIRE & opt)
            convert.low();
        else if (FOUR_WIRE & opt)
            Pin::high();
    }
	
    delay_ns(2300); // 2.3 us
	
    if (THREE_WIRE & opt && NO_BUSY_INDICATOR & opt)
        Pin::low();
	
    // read in the actual voltage reading(s) over SPI
    read_uv(dst);
	
	Pin::low();
    delay_us(5);
}