/* Should repeatedly print "hello, world!" and a running count to the serial
 * console. When the USER button is pressed, it the contents of the RX buffer
 * will be flushed. When a carriage return ('\r') or line feed ('\n') are
 * received, the contents of the buffer will be printed.
 *
 * To setup putty to monitor, choose:
 *      Session:
 *          Connection type:    Serial
 *          Serial line:        The lower of the two FTDI COM ports (if using
 *              newer NU32s). Use device manager to find the COM ports.
 *          Speed:              9600
 *      Connection:
 *          Serial:
 *              Data bits:      8
 *              Stop bits:      1
 *              Parity:         None
 *              Flow control:   None
 */

#include <string.h>
#include "nu32.h"
#include "serial.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct serial ser, *serp = &ser;
    BYTE delims[] = {'\r', '\n'};
    const char *str = "hello, world!\r\n";
    uint32_t ui = 0;
    
    nu32_init(80000000);

    serial_new(serp, NU32_UART_MODULE, 9600, NO_UART_INTERRUPT,
            INT_PRIORITY_DISABLED,
            0,
            UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1,
            UART_ENABLE_PINS_TX_RX_ONLY,
            UART_PERIPHERAL|UART_TX|UART_RX, delims, sizeof(delims));

    while (1) {
        char rxBuf[100] = {0};
        if (!(ui%10))
            serp->op->tx(serp, str, strlen(str));
        serp->op->printf(serp, "count:\t%d\r\n", ui);
        if (!serp->op->rx(serp, rxBuf, sizeof(rxBuf))) {
            serp->op->tx(serp, rxBuf, sizeof(rxBuf));
            serp->op->tx(serp, "\r\n", 2);
            delay(2);
        }
        if (!PORTReadBits(NU32_USER_PORT, NU32_USER_PIN)) {
            serp->op->flushRxBuf(serp, rxBuf, sizeof(rxBuf));
            serp->op->tx(serp, rxBuf, sizeof(rxBuf));
            serp->op->tx(serp, "\r\n", 2);
            delay(2);
        }

        ui++;
    }

    return 0;
}