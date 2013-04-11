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

Open and compile projects in MPLAB X

Upload binaries to PICs with MPLAB X's IPE


Usage
----
Turn on boards

Projects
----
Unfortunately Chris implemented <i>absolutely everything</i> in
<a href="http://en.wikipedia.org/wiki/X_Macro">X macros</a>. Understanding their
syntax is required for reading any code. Peruse with caution.

### Project names beginning with `car_` go on the car.
`car_bms` — lots of BMS

`car_driver_controls` — setup pins: analog in, digital in, digital out.

`car_steering_wheel` — indefinitely update Button values. Should also Animate buttons. Should also draw LCD.
* We call `btn_update` from `button.h`. We also define `_PIN` in `_LED`.

### Otherwise-named projects are development utilities.

`get_tempSensors` — Utility to get temperature serial numbers, to populate the table in the BMS code.
* The temperature sensors are on a 1-wire bus. We need to know which sensor corresponds to which module.

`mppt_can` — Utility to tell MPPTs to begin sending data.
* MPPTs are dumb. We need to tell them to begin to send data. (On the car, this is done by the BMS)

`mppt_upload` — Utility to upload the MPPT binary.
* Requires an MPPT hex blob, which the company sends us.

NU API
----
All microcontroller internal and pin functionality is wrapped. Sensor interfaces
are written over our wrappings. Projects utilize these interfaces.

### NU32
`nu32.h` — setup NU32 LEDs and UARTs. Possibly does some ADC initialization.

`led.h` — control NU32 LEDs

### Internal & Pin-communication abstractions
`wdt.h` — wrap enable/disable clearing internal WatchDogTimer. We disable during trips

`flash.h` — wrap programming microcontroller flash memory

`timer.h` — wrap timer, and delay functions

`pinctl.h` — wraps PIC32 pins as NU_PIN.
* declaring, setting digital/analog in/out, reading/setting bits, clearing, toggling.

`onewire.h` — Wrap pin-communication with 1-wire pins, on top of `pinctl.h`

`can.h` — Wrap CAN peripheral library: setting up pins, adding channels, TX/RX

`serial.h` — Wraps PIC32 UART_MODULE for serial comm

`serial_async.h` — Same as above, but somehow async

`spi.h` — Wraps SPI functions: declaring SPI pins, setting up, reading, and writing.

#### Hardware interfaces
`button.h` — abstracts declaring buttons, updating value, and checking value
* `struct btn` holds NU_PIN, and current button value. `btn_update(b)` updates `b->debounce`.

`ltc6803.h` — SPI Voltage monitoring chip

`ad7685.h` — SPI Analog-to-Digital Converter used on the BMS current sensor

`nokia5110.h` — SPI LCD.

`mcp49x2.h` — SPI Digital-to-Analog Converter (unused)

`ds18x20.h` — onewire Temperature sensor

### Supporting types
`can_all.h` — Define all CAN packets

`errorcodes.h` — our error codes

`error_reporting.h` — generalized error reporting, to multiple devices
* Attach error reporting devices, broadcast error to all devices

`mppt_race.h` — unknown, possibly spam MPPTs with requests

`async_io.h` — asynchronously queue messages to circular buffer, ISRs

#### basic types
`list.h` — provides doubly linked list

`safestring.h` — provides `strlcpy` and `strlcat` for our Cstrings

`crc.h` — cyclic redundancy checks, necessary for network data

### Inline Math and aliases
`hais50p.h` — Convert HAIS-50P (via `ad7685.h`) reported voltage to current

`byteorder.h` — byteswapping facilities

`compiler.h` — our ridiculous aliases to GCC extensions

`lock.h` — wrap atomic memory access GCC extensions

`nu_types.h` — equally ridiculous "faster to type" aliases to primitive types

`utility.h` — BUSY_FOR, CLAIM_PIN, other useful functions
* Includes (inline) arithmetic, bits, data, and preprocessor utility functions. And static assertions
* Includes `stdlib.h` and `compiler.h`

### Testing

`minunit.h` — unit testing framework

Coding Conventions
----
Remember: There is no <del>spoon</del> heap.

Many interfaces are implemented with X Macros for speed and modularity.
Some implement object-oriented behavior around structs. This is done with 4 DEFINEs:
* `struct nu_*` — the struct to be wrapped
* `(NU_)*_INIT` are struct initialization blobs. Set a struct equal to the return value.
* `(NU_)*` which accompany these INITs are full declarations, and make use of the corresponding (NU_)*_INIT
* `(NU_)INIT_*` are function-like, accepting a struct argument and assigning to the struct's members. (unused)

For example:
* `struct nu_pin` is a struct with fields `ltr` and `num`
* `NU_PIN_INIT(ltr,num)` expands to a struct initializer, `{(ltr), (num)}`
* `NU_PIN(name, ltr, num)` is a struct declaration: `struct nu_pin name = NU_PIN_INIT(ltr, num)`
* `NU_INIT_PIN(struct nu_pin *p, IoPortId ltr, u32 num)` is just a function.

For indentation: K&R style is used. For function signatures: Linux style is used. For all public symbols: the `nu_` namespace is used. For function, struct, and variable names: `nu_lowercase_with_underscores` is used. Macros are `NU_ALL_CAPS`. For example:

```C
static ALWAYSINLINE void
NU_INIT_PIN(struct nu_pin *p, IoPortId ltr, u32 num)
{
    p->ltr = ltr;
    p->num = num;
}
```




