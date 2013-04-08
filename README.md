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

### Hardware objects

`button.h` — abstracts steering wheel buttons

`ltc6803.h` — our Voltage monitoring chips
