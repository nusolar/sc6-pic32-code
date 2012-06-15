#ifndef __NU_NU32_H
#define __NU_NU32_H

#include <stdint.h>
#ifdef PC_BUILD
    #include <stdio.h>
#else
    #include <plib.h>
#endif

extern uint32_t sys_clk_hz;

#define NU32_LED0_PORT      IOPORT_G
#define NU32_LED0_PIN       BIT_12
#define NU32_LED1_PORT      IOPORT_G
#define NU32_LED1_PIN       BIT_13

#define NU32_SWITCH_PORT    IOPORT_G
#define NU32_SWITCH_PIN     BIT_6

#define NU32_UART_MODULE    UART3   /* = UART2A */

/* Initialize and configure:
 *   - The PIC32 for multi-vector interrupts, running at the specified frequency
 *   - The LED ports on G12 and G13 on the NU32 board
 *   - G6 as input
 */
int32_t
nu32_init(uint32_t sysClkHz);

/* Initialize the serial port 
 * Note: the NU32v2 is hard wired to use UART3 (= UART2A)
 */
void
initSerialNU32v2(uint32_t serialBaudRate);

/* Write a string over the serial port */
void
WriteString(UART_MODULE id, const char *string);

/* Put a character over the serial port */
void
PutCharacter(UART_MODULE id, char character);

#endif
