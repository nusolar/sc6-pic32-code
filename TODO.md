TODO
====

steering wheel
----

* add all button PINs

* If clock/IO lags while btn.debounce > btn.thresh, could take up to 100 pin reads needed to realize button is no longer pressed!

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

wavesculptor20 and CAN
----

* fix ws20.c

* fix CAN

Error reporting
----

* implement wdt.c

SPI Hardware
----

* fix nokia5110.h enum errors
