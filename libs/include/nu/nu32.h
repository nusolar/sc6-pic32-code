#ifndef NU_NU32_H
#define NU_NU32_H 1

/**
 * @file
 * Common variables and utilities for Nick Marchuck's NU32 boards.
 *
 * Supports NU32v1 and NU32v2
 *
 * @sa http://hades.mech.northwestern.edu/index.php/NU32:_Introduction_to_the_PIC32
 */

#include "compiler.h"
#include "led.h"
#include "param.h"
#include "pinctl.h"
#include "serial.h"

#if NU32_V == 1
static const NU_LED(nu32_led1,    IOPORT_A, BIT_4);
static const NU_LED(nu32_led2,    IOPORT_A, BIT_5);
#define nu32_led0   nu32_led2   /* preserve code compatability */
static const NU_PIN(nu32_switch,  IOPORT_C, BIT_13);
static const SERIAL(nu32_serial, UART1);
#define nu32_serial1 nu32_serial
static const SERIAL(nu32_serial2, UART4);  /* used by bootloader */
#elif NU32_V == 2
static const NU_LED(nu32_led0,    IOPORT_G, BIT_12);
static const NU_LED(nu32_led1,    IOPORT_G, BIT_13);
#define nu32_led2   nu32_led0       /* preserve code compatability */
static const NU_PIN(nu32_switch,  IOPORT_G, BIT_6);
static const SERIAL(nu32_serial2, UART3);    /* UART2A */
static const SERIAL(nu32_serial, UART1);
#define nu32_serial1 nu32_serial    /* preserve code compatability */
#endif

#define nu32_user   nu32_switch

/**
 * @brief Sets up the clock, leds, and serial ports on the NU32 board
 *
 * Due to the constructor attribute, this setup function is called automatically
 * before main(). Because it is weakly linked, it may be overriden by creating
 * a custom implementation with the same function signature.
 *
 * @warning If nu32.c is not compiled (included in the MPLAB X project), then
 * nu32_setup() will <b>not</b> be automatically called. In other words, if the
 * linker does not find the symbol it will silently omit the constructor call.
 */
WEAK CONSTRUCTOR(()) void
nu32_setup(void);

void
nu32_setup_leds(void);

#endif
