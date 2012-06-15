#ifndef __NU_AD7685_H
#define __NU_AD7685_H

#include <stdint.h>
#include "errorcodes.h"
#include "spi.h"

enum WireConfiguration {
    THREE_WIRE,
    FOUR_WIRE,
    CHAIN_MODE
};

enum BusyIndicator {
    NO_BUSY_INDICATOR,
    USE_BUSY_INDICATOR
};

struct ad7685 {
    struct spiPort              spiPort;

    const struct vtbl_ad7685    *op;

    uint32_t cnvPinNum;
    uint32_t numDevices;
    IoPortId cnvPinLtr;

    enum WireConfiguration WireConfig;
    enum BusyIndicator UseBusyIndicator;
};

struct vtbl_ad7685 {
    /**
     * Returns the ADC voltage reading.
     * @param   self        The ad7685 instance on which to operate.
     * @param   voltages    An array of floats to hold the converted voltages.
     *                      The number of elements should match the number of
     *                      AD7685 devices (self->numDevices).
     * @return  0 on success.
     * 
     * Example:
     * @snippet ad7685.c convertAndReadVolts example
     */
    int32_t (*convertAndReadVolts)    (struct ad7685 *self, float *voltages);
};

/**
 * Initializes one or more AD7685 devices with no chip-select pin (refer to the
 * AD7685 documentation to determine which modes require the use of chip-select.
 * @return  0 on success.
 *
 * Example:
 * @snippet ad7685.c ad7685_new example
 */
int32_t
ad7685_new(struct ad7685 *self, SpiChannel chn, IoPortId cnvPinLtr, uint32_t cnvPinNum, uint32_t numDevices,
            enum WireConfiguration WireConfig, enum BusyIndicator UseBusyIndicator);

/**
 * Initializes one or more AD7685 devices with a chip-select pin (refer to the
 * AD7685 documentation to determine which modes require the use of chip-select.
 * @return  0 on success.
 *
 * Example:
 * @snippet ad7685.c ad7685_CS_new example
 */
int32_t
ad7685_CS_new(struct ad7685 *self, SpiChannel chn, IoPortId cnvPinLtr, uint32_t cnvPinNum, uint32_t numDevices,
            enum WireConfiguration WireConfig, enum BusyIndicator UseBusyIndicator,
            IoPortId csPinLtr, uint32_t csPinNum);

#endif
