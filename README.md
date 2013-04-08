sc6-pic32-code
==============

NUSolar SC6 PIC32 code


Prerequisites
----
MPLAB X IDE ≥ 1.0

PICkit 2 or 3


Installation
----
Clone repository

Open and compile in MPLAB X

Upload binaries to PICs with MPLAB X's IPE


Usage
----
Turn on boards

Projects
----

Unfortunately Chris implemented <i>absolutely everything</i> in
<a href="http://en.wikipedia.org/wiki/X_Macro">X macros</a>.
Peruse with caution.

### Project names beginning with `car_` go on the car.

`car_bms` — lots of BMS

`car_driver_controls` — setup pins: analog in, digital in, digital out.

`car_steering_wheel` — indefinitely update Button values. Should also Animate buttons. Should also draw LCD.

* We primarily call `btn_update` from `button.h`. We also define `_PIN` in `_LED`

### Otherwise-named projects are development utilities.

`get_tempSensors` — Utility to get temperature serial numbers, to populate the table in the BMS code.

* The temperature sensors are on a 1-wire bus. We need to know which sensor corresponds to which module.

`mppt_can` — Utility to tell MPPTs to begin sending data.

* MPPTs are dumb. We need to tell them to begin to send data. (On the car, this is done by the BMS)

`mppt_upload` — Utility to upload the MPPT binary.

* Requires an MPPT hex blob, which the company sends us.

Headers
----

### NU32

`nu32.h` — setup NU32 LEDs and UARTs. Possibly does some ADC initialization.

`led.h` — control NU32 LEDs

### Hardware objects

`pinctl.h` — wraps PIC32 pins as NU_PIN.

* declaring, setting digital/analog in/out, reading/setting bits, clearing, toggling.

`button.h` — abstracts declaring buttons, updating value, and checking value

* `struct btn` holds NU_PIN, and current button value. `btn_update(b)` updates `b->debounce`.

`can.h` — Wrap CAN, abstracting adding channels and TX/RX

`serial.h` — Wraps PIC32 UART_MODULE for serial comm

`spi.h` — SPI functions. Abstracts declaring SPI pins, setting up, reading, and writing.

`ltc6803.h` — SPI Voltage monitoring chip

`ad7685.h` — SPI Analog-to-Digital Converter used on the BMS current sensor

`nokia5110.h` — SPI LCD.

`wdt.h` — no idea

### Useful functions

`can_all.h` — Define all CAN packets

`errorcodes.h` — our error codes

`error_reporting.h` — report errors, presumably to CAN

`list.h` — provides doubly linked list

`safestring.h` — provides `strlcpy` and `strlcat` for our Cstrings

`arith.h` — some (inline) arithmetic functions

`nu_types.h` — our ridiculous "quicker to type" aliases to primitive types

`compiler.h` — equally ridiculous aliases to GCC extensions

`utility.h` — BUSY_FOR, CLAIM_PIN, other useful functions

* Includes bits, data, and preprocessor utility functions. And static assertions

* Includes `stdlib.h` and `compiler.h`

`crc.h` — cyclic redundancy checks, for network data



