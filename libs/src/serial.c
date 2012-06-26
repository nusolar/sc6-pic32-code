#include "../include/serial.h"

static int32_t
serial_init(struct serial *self, unsigned int baudrate, enum ModuleInterrupt UseInterrupt,
                INT_PRIORITY intPriority, UART_FIFO_MODE InterruptModes,
                UART_LINE_CONTROL_MODE LineControlModes, UART_CONFIGURATION UARTConfig,
                UART_ENABLE_MODE EnableModes);

static int32_t
tx (const struct serial *self, const void *data, size_t len);

static int32_t
rx (struct serial *self, void *dst, size_t len);

static int32_t
flushRxBuf (struct serial *self, void *dst, size_t len);

static int32_t
serialprintf (const struct serial *self, const char *fmt, ...) __attribute__((format(printf,2,3)));

const struct vtblSerial serialOps = {
    .printf     = &serialprintf,
    .tx         = &tx,
    .rx         = &rx,
    .flushRxBuf = &flushRxBuf,
};

static int32_t
serial_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg);

static const struct vtblError_reporting_dev serial_erd_ops = {
    .report         = &serial_report,
    .resetErrState  = NULL,
};

static int32_t
resetRxBuf(struct serial *self)
{
    if (self == NULL)
        return -ENULPTR;

    memset(self->rxBuf, 0, sizeof(self->rxBuf));
    self->currentBufPos = 0;
    self->rxBufFull = FALSE;

    return 0;
}

int32_t
serial_new(struct serial *self, UART_MODULE serialModule, unsigned int baudrate, enum ModuleInterrupt UseInterrupt,
                INT_PRIORITY intPriority, UART_FIFO_MODE InterruptModes,
                UART_LINE_CONTROL_MODE LineControlModes, UART_CONFIGURATION UARTConfig,
                UART_ENABLE_MODE EnableModes, const BYTE *delims, size_t numDelims)
{
    uint32_t ui;
    const size_t maxDelims = MAX_DELIMS;

    if (self == NULL)
        return -ENULPTR;

    self->op        = &serialOps;
    self->erd.op    = &serial_erd_ops;
    self->serialModule = serialModule;
    resetRxBuf(self);

    self->numDelims = MIN(maxDelims, numDelims);
    for (ui = 0; ui < self->numDelims; ui++)
        self->delims[ui] = delims[ui];

    return serial_init(self, baudrate, UseInterrupt, intPriority, InterruptModes,
                        LineControlModes, UARTConfig, EnableModes);
}

static int32_t
serial_init(struct serial *self, uint32_t baudrate, enum ModuleInterrupt UseInterrupt,
                INT_PRIORITY intPriority, UART_FIFO_MODE InterruptModes,
                UART_LINE_CONTROL_MODE LineControlModes, UART_CONFIGURATION UARTConfig,
                UART_ENABLE_MODE EnableModes)
{
    INT_VECTOR intVect;
    INT_SOURCE intSrc;

    if (self == NULL)
        return -ENULPTR;

    UARTConfigure(self->serialModule, UARTConfig);
    UARTSetFifoMode(self->serialModule, InterruptModes);
    UARTSetLineControl(self->serialModule, LineControlModes);
    UARTSetDataRate(self->serialModule, sys_clk_hz, baudrate);
    UARTEnable(self->serialModule, UART_ENABLE_FLAGS(EnableModes));

    if (UseInterrupt) {
        switch(self->serialModule) {
            case UART1: 
                intVect = INT_UART_1_VECTOR;
                intSrc = INT_U1RX;
                break;
            case UART2:
                intVect = INT_UART_2_VECTOR;
                intSrc = INT_U2RX;
                break;
            case UART3:
                intVect = INT_UART_3_VECTOR;
                intSrc = INT_U3RX;
                break;
            case UART4:
                intVect = INT_UART_4_VECTOR;
                intSrc = INT_U4RX;
                break;
            case UART5:
                intVect = INT_UART_5_VECTOR;
                intSrc = INT_U5RX;
                break;
            case UART6:
                intVect = INT_UART_6_VECTOR;
                intSrc = INT_U6RX;
                break;
            case UART_NUMBER_OF_MODULES: 
            default:
                break;
        }
        INTEnable(intSrc, INT_ENABLED);
        INTSetVectorPriority(intVect, intPriority);
        INTSetVectorSubPriority(intVect, INT_SUB_PRIORITY_LEVEL_0);
    }

    return 0;
}

static int32_t
serial_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg)
{
    struct serial *serialSelf;
    char errInfoBuf[1024], contextBuf[1024];

    if (!self || !file || !expr || !errName || !fmtdMsg)
        return -ENULPTR;

    serialSelf = error_reporting_dev_to_serial(self);

    printErrInfo(errInfoBuf, sizeof(errInfoBuf), priority, errNum, errName);
    printContextInfo(contextBuf, sizeof(contextBuf), expr, file, line);

    serialSelf->op->printf(serialSelf, "%s:\n", errInfoBuf);
    serialSelf->op->printf(serialSelf, "\t%s\n", fmtdMsg);
    serialSelf->op->printf(serialSelf, "\tat %s\n", contextBuf);

    return 0;
}

static int32_t
tx (const struct serial *self, const void *data, size_t len)
{
    size_t ui;

    if (self == NULL || data == NULL)
        return -ENULPTR;
    
    for (ui = 0; ui < len; ui++) {
        while (!UARTTransmitterIsReady(self->serialModule))
            ;   /* do nothing */
        UARTSendDataByte(self->serialModule, ((const BYTE *)data)[ui]);
    }

    while (!UARTTransmissionHasCompleted(self->serialModule))
        ;   /* do nothing */

    return 0;
}

static int32_t
bufDelimCpy (const struct serial *self, uint32_t delimPos, void *dst, size_t len)
{
    uint32_t ui, uj;

    if (self == NULL || dst == NULL)
        return -ENULPTR;

    ui = (self->rxBufFull == TRUE) ?
        (delimPos+1)%ARRAY_SIZE(self->rxBuf) : 0;
    uj = 0;
    for ( ; ui != delimPos; ui = (ui+1)%ARRAY_SIZE(self->rxBuf)) {
        if (self->rxBuf[ui]) {
            if (uj < len)
                ((BYTE *)dst)[uj++] = self->rxBuf[ui];
            else
                return -ETRUNCATED;
        }
    }


    return 0;
}

static int32_t
matchesDelim(const struct serial *self, BYTE val)
{
    size_t i;
    for (i = 0; i < self->numDelims; i++)
        if (val == self->delims[i])
            return 1;

    return 0;
}

static int32_t
rx (struct serial *self, void *dst, size_t len)
{
    int32_t err = 0;
    BOOL rxMsgSuccess = FALSE;

    if (self == NULL || dst == NULL)
        return -ENULPTR;

    while (UARTReceivedDataIsAvailable(self->serialModule)) {
        self->rxBuf[self->currentBufPos] = UARTGetDataByte(self->serialModule);
        if (self->currentBufPos == ARRAY_SIZE(self->rxBuf)-1)
            self->rxBufFull = TRUE;

        if (matchesDelim(self, self->rxBuf[self->currentBufPos])) {
            rxMsgSuccess = TRUE;

            err = bufDelimCpy(self, self->currentBufPos, dst, len);

            resetRxBuf(self);
            break;
        }

        self->currentBufPos = (self->currentBufPos + 1) % ARRAY_SIZE(self->rxBuf);
    }

    if (rxMsgSuccess == FALSE)
        err = -ENODATA;

    return err;
}

static int32_t
flushRxBuf (struct serial *self, void *dst, size_t len)
{
    if (self == NULL)
        return -ENULPTR;

    if (dst != NULL && len >= 1)
        memcpy(dst, self->rxBuf, MIN(sizeof(self->rxBuf), len));
    
    return resetRxBuf(self);
}

static int32_t
serialprintf (const struct serial *self, const char *fmt, ...)
{
    va_list fmtargs;
    char buffer[1024];

    if (self == NULL || fmt == NULL)
        return -ENULPTR;

    va_start(fmtargs, fmt);
    vsnprintf(buffer, sizeof(buffer)-1, fmt, fmtargs);
    va_end(fmtargs);

    return tx(self, buffer, strlen(buffer));
}
