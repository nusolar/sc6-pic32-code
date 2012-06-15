/**
 * @file    NUSPI_new.h
 * @brief   An object-oriented interface to SPI functionality on the pic32.
 * @author  Chris Yungmann
 */

#ifndef __NU_SPI_H
#define __NU_SPI_H

#include <plib.h>
#include <stdint.h>
#include "errorcodes.h"
#include "nu32.h"
#include "timer.h"

enum autoCsPin {
    AUTO_CS_PIN_DISABLE = 0,
    AUTO_CS_PIN_ENABLE
};

struct spiPort {
    const struct vtblSpiPort *op;   /**< A pointer to the static virtual
                                     *  function table, which in turn holds
                                     *  pointers to the possible operations
                                     *  for an spiPort
                                     */
    unsigned int csPinNum;          /**< The chip select pin number.
                                     *  BIT_6 for pin number 6. These can be
                                     *  found in ports.h
                                     */
    SpiChannel chn;                 /**< The SPI channel to use */
    enum autoCsPin autoCSPinEnable;
    IoPortId csPinLetter;           /**< The chip select pin letter. Ex:
                                     *  IOPORT_G. These can be found in
                                     *  ports.h, part of MicroChip's
                                     *  peripheral library.
                                     */
};

/**
 * A virtual function table, or vtbl, that holds pointers to the functions
 * for an spiPort.
 */
struct vtblSpiPort {
    int32_t (*tx)          (const struct spiPort *self, const void *data, size_t len);
    int32_t (*rx)          (const struct spiPort *self, void *dest, size_t len);
    int32_t (*txStr)       (const struct spiPort *self, const char *str);
    int32_t (*driveCSHigh) (const struct spiPort *self);
    int32_t (*driveCSLow)  (const struct spiPort *self);
};

int32_t
SPI_new(struct spiPort *self, SpiChannel chn, uint32_t bitrate,
        SpiOpenFlags oFlags);

int32_t
SPI_CS_new(struct spiPort *self, SpiChannel chn, uint32_t bitrate,
        SpiOpenFlags oFlags, enum autoCsPin autoCSPinEnable,
        IoPortId csPinLetter, uint32_t csPinNum);

#endif
