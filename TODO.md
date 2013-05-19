TODO
====

NU++ API
----
BMS and BMS error reporting are unimplemented. I leave that to C for now.

Steering Wheel:
* Draw highest Battery temp, Cruise Control velocity, and percent charge remaining to LCD

Driver Controls:
* Cruise control
* Apply Exponential Smoothing to Acceleration Pedal, and sent Motor Current
* Scale Motor Current with BMS maximum-current packets, to prevent tripping when driver floors pedal and when batteries are almost empty.

NU API (highest priority first)
----
* pinctl (done?)
* timer
* w1
* ds18x20
* ad7685
* hais50p
* ltc6803
* wdt
* flash
* CAN
* ws20
* mppt
* async_io

#### ws20
* remove old struct/op notation.
* ERD vtbl is never declared.
* both old and new CAN address names are used.
* enum PhaseSeg2TimeSelect and SampleTimes don't exist.
* nu_ws20_setup and nu_ws20_new_easy aren't implemented. nu_ws20_init() is used instead.

#### CAN
* can_all.h doesn't compile
* many pedantic "ignoring result of" errors.
* the longstanding `container_of` error
* the ERD pointer type error.
* `union can_anyFrame` is referenced but never defined.
* why is `CAN_BIT_CONFIG *default_cfg` commented out?
* `nu_can_new_easy()` isn't implemented

#### Error reporting
* fix error report definition inconsistency

#### SPI Hardware
* fix errors in ltc6803/ad7685/everything

### Projects (highest priority first)
* BMS
* driver controls
* steering wheel

#### steering wheel
* add all button PINs
* btn_update call frequency may need tweeking

Algorithm sketch:
* check if btn_pressed
* determine all Pressed Buttons
* send CAN packet of pressed buttons each cycle
* Implement LEDs
* Draw speed on LCD (LCD UNIMPLEMENTED)

#### driver controls
* read pins
* send CAN status updates
* send CAN motor controller commands
* Draw LCD
* listen for commands, implement
