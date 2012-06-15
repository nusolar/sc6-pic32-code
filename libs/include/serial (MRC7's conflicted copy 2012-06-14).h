#ifndef __NU_SERIAL_H
#define __NU_SERIAL_H

#include <string.h>
#include <plib.h>
#include "errorcodes.h"
#include "error_reporting.h"
#include "nu32.h"
#include "timer.h"

#define RXBUF_SIZE  50
#define MAX_DELIMS  5

#define error_reporting_dev_to_serial(erdp)   \
        container_of((erdp), struct serial, erd)

enum ModuleInterrupt {
    NO_UART_INTERRUPT,
    USE_UART_INTERRUPT,
};

struct serial {
    const struct vtblSerial *op;

    struct error_reporting_dev erd;

    UART_MODULE     serialModule;
    uint32_t        currentBufPos;
    BYTE            rxBuf[RXBUF_SIZE];
    BYTE            delims[MAX_DELIMS];
    size_t          numDelims;
    BOOL            rxBufFull;
};

struct vtblSerial {
    int32_t (*printf)       (const struct serial *self, const char *fmt, ...) __attribute__((format(printf,2,3)));
    int32_t (*tx)           (const struct serial *self, const void *data, size_t len);
    int32_t (*rx)           (struct serial *self, void *dst, size_t len);
    int32_t (*flushRxBuf)   (struct serial *self, void *dst, size_t len);
};

int32_t
serial_new(struct serial *self, UART_MODULE serialModule, unsigned int baudrate, enum ModuleInterrupt UseInterrupt,
                INT_PRIORITY intPriority, UART_FIFO_MODE InterruptModes,
                UART_LINE_CONTROL_MODE LineControlModes, UART_CONFIGURATION UARTConfig,
                UART_ENABLE_MODE EnableModes, const BYTE *delims, size_t numDelims);

#endif