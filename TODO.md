TODO
====

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

wavesculptor20
----
* fix ws20.c

CAN
---
* many pedantic "ignoring result of" errors.
* the longstanding `container_of` error
* the ERD pointer type error.
* `union can_anyFrame` is referenced but never defined.
* `CAN_BIT_CONFIG *default_cfg` is never declared either.
* `nu_can_new_easy()` isn't implemented


Error reporting
----
* fix error report definition inconsistency

SPI Hardware
----

* fix nokia5110.h enum errors
