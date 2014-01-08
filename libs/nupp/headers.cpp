// printed circuit boards
#include "nupp/board/bms.hpp"
#include "nupp/board/bps.hpp"
#include "nupp/board/nu32.hpp"
#include "nupp/board/output_board.hpp"
#include "nupp/board/pedals.hpp"

// PCB components
#include "nupp/component/ad7685.hpp"
#include "nupp/component/hais.hpp"
#include "nupp/component/ltc6803.hpp"
#include "nupp/component/ltc6804.hpp"
#include "nupp/component/nokia5110.hpp"
#include "nupp/component/ulcd28pt.hpp"
#include "nupp/component/button.hpp"
#include "nupp/component/led.hpp"
#include "nupp/component/ds18x20.hpp"

// comm protocols
#include "nupp/can.hpp"
#include "nupp/onewire.hpp"
#include "nupp/serial.hpp"
#include "nupp/spi.hpp"
#include "nupp/usbhid.hpp"

// basic microcontroller functions
#include "nupp/crc.hpp"
#include "nupp/param.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/wdt.hpp"

// stack-based Standard Library
#include "nupp/allocator.hpp"
#include "nupp/array.hpp"
#include "nupp/bitset.hpp"
#include "nupp/enum.hpp"
#include "nupp/errorcodes.hpp"
#include "nupp/lowpassfilter.hpp"
#include "nupp/string.hpp"
#include "nupp/stream.hpp"
