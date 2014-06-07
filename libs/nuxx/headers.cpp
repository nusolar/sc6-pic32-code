// printed circuit boards
#include "nuxx/board/bps.hpp"
#include "nuxx/board/DriverControls.hpp"
#include "nuxx/board/test.hpp"
#include "nuxx/board/nu32.hpp"

// Automotives systems
#include "nuxx/comm/currentsensor.hpp"
#include "nuxx/comm/tempsensor.hpp"
#include "nuxx/comm/voltagesensor.hpp"

// PCB components
#include "nuxx/component/ad7685.hpp"
#include "nuxx/component/hais.hpp"
#include "nuxx/component/ltc6803.hpp"
#include "nuxx/component/ltc6804.hpp"
#include "nuxx/component/nokia5110.hpp"
#include "nuxx/component/ulcd28pt.hpp"
#include "nuxx/component/button.hpp"
#include "nuxx/component/led.hpp"
#include "nuxx/component/ds18x20.hpp"

// PLIB protocols
#include "nuxx/peripheral/can.hpp"
#include "nuxx/peripheral/onewire.hpp"
#include "nuxx/peripheral/serial.hpp"
#include "nuxx/peripheral/spi.hpp"
#include "nuxx/peripheral/usbhid.hpp"
#include "nuxx/peripheral/pin.hpp"

// basic system services
#include "nuxx/param.hpp"
#include "nuxx/timer.hpp"
#include "nuxx/wdt.hpp"
#include "nuxx/crc.hpp"

// stack-based Standard Library
#include "nuxx/array.hpp"
#include "nuxx/bitset.hpp"
#include "nuxx/enum.hpp"
#include "nuxx/errorcodes.hpp"
#include "nuxx/lowpassfilter.hpp"
#include "nuxx/string.hpp"
#include "nuxx/stream.hpp"
