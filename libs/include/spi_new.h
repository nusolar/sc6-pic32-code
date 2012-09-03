#ifndef NU_SPI_NEW_H
#define NU_SPI_NEW_H

#include <stdint.h>

typedef int32_t (*csControlFunc)(const struct spiPort *self, uint32_t driveHigh);
typedef int32_t (*spiSendByteFunc)(const struct spiPort *self, uint8_t byte);
typedef int32_t (*spiInitFunc)(const struct spiPort *self);

enum autoCsPin {
    AUTO_CS_PIN_DISABLE,
    AUTO_CS_PIN_ENABLE
};

struct spiPort {
    const struct vtblSpiPort    *op;
    csControlFunc               csControl;
    spiSendByteFunc             spiSendByte;
    enum autoCsPin              autoCsPinEnable;
};

struct vtblSpiPort {
    int32_t (*tx)          (const struct spiPort *self, const void *data, size_t len);
    int32_t (*rx)          (const struct spiPort *self, void *dest, size_t len);
    int32_t (*txStr)       (const struct spiPort *self, const char *str);
    int32_t (*driveCSHigh) (const struct spiPort *self);
    int32_t (*driveCSLow)  (const struct spiPort *self);
};

int32_t
SPI_new(struct spiPort *self, int32_t (*csControl)(uint32_t driveHigh), int32_t (*spiSendByte)(uint8_t byte));

#endif