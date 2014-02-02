// printed circuit boards
#include "nupp/board/bps.hpp"
#include "nupp/board/oldbps.hpp"
#include "nupp/board/pedals.hpp"
#include "nupp/board/output_board.hpp"
#include "nupp/board/nu32.hpp"

// Automotives systems
#include "nupp/comm/currentsensor.hpp"
#include "nupp/comm/tempsensor.hpp"
#include "nupp/comm/voltagesensor.hpp"

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

// PLIB protocols
#include "nupp/peripheral/can.hpp"
#include "nupp/peripheral/onewire.hpp"
#include "nupp/peripheral/serial.hpp"
#include "nupp/peripheral/spi.hpp"
#include "nupp/peripheral/usbhid.hpp"
#include "nupp/peripheral/pinctl.hpp"

// basic system services
#include "nupp/crc.hpp"
#include "nupp/param.hpp"
#include "nupp/timer.hpp"
#include "nupp/wdt.hpp"

// stack-based Standard Library
#include "nupp/array.hpp"
#include "nupp/bitset.hpp"
#include "nupp/enum.hpp"
#include "nupp/errorcodes.hpp"
#include "nupp/lowpassfilter.hpp"
#include "nupp/string.hpp"
#include "nupp/stream.hpp"
