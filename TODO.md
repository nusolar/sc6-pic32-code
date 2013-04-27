TODO
====

NUSolar API (highest priority first)
----
* param
* error_reporting
* serial (get in semi-stable state for use in nu32)
* led
* pinctl (done?)
* nu32
* timer
* spi
* nokia5110
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

Projects (highest priority first)
----
* BMS
* driver controls
* steering wheel

steering wheel
----

* add all button PINs
* btn_update call frequency may need tweeking

Algorithm sketch:
* check if btn_pressed
* determine all Pressed Buttons
* send CAN packet each cycle, of pressed buttons
* Implement LEDs
* Draw speed on LCD

driver controls
----
* read pins
* send CAN status updates
* send CAN motor controller commands
* Draw LCD
* listen for commands, implement

ws20
----
* remove old struct/op notation.
* ERD vtbl is never declared.
* both old and new CAN address names are used.
* enum PhaseSeg2TimeSelect and SampleTimes don't exist.
* nu_ws20_setup and nu_ws20_new_easy aren't implemented. nu_ws20_init() is used instead.
* can_all.h doesn't compile

CAN
---
* many pedantic "ignoring result of" errors.
* the longstanding `container_of` error
* the ERD pointer type error.
* `union can_anyFrame` is referenced but never defined.
* why is `CAN_BIT_CONFIG *default_cfg` commented out?
* `nu_can_new_easy()` isn't implemented


Error reporting
----
* fix error report definition inconsistency

SPI Hardware
----
* fix nokia5110.h enum errors
