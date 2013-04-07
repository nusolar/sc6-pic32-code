sc6-pic32-code
==============

NUSolar SC6 PIC32 code

Prerequisites
-------------
MPLAB X IDE ≥ 1.0

PICkit 2 or 3

Installation
------------
Clone repository

Open in MPLAB X

Load onto PICs

Projects
--------

<b>Project names beginning with `car_` go on the car.</b>

`car_bms` —

`car_steering_wheel` — indefinitely update Button values. Should also Animate buttons. Should also draw LCD.

`get_tempSensors` — Utility to get temperature serial numbers, to populate the table in the BMS code.

* The temperature sensors are on a 1-wire bus. We need to know which sensor corresponds to which module.

`mppt_can` — Utility to tell MPPTs to begin sending data.

* MPPTs are dumb. We need to tell them to begin to send data. (On the car, this is done by the BMS)

`mppt_upload` — Utility to upload the MPPT binary.

Usage
-----
Turn on PICs
