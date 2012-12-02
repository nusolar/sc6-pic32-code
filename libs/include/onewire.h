/**
 * NUSolar Object-Oriented 1-Wire Library
 */

#ifndef __NU_ONEWIRE_H
#define __NU_ONEWIRE_H

#include <limits.h>
#include <plib.h>
#include <stdint.h>
#include "crc.h"
#include "errorcodes.h"
#include "timer.h"
#include "utility.h"

union romCode {
    struct {
        BYTE familyCode;
        BYTE serialNum[6];
    };
    BYTE byteArr[7];
};

struct oneWire {
    struct {
        uint32_t    lastDiscrepancyBit;
        uint32_t    lastFamilyDiscrepBit;
        BOOL        prevSearchWasLastDevice;
        union {
            BYTE        dataBytes[8];
            struct {
                union romCode   rc;
                BYTE            crc;
            };
        } romCodeAndCrc;
    } searchState;

    const struct vtblOneWire    *op;

    uint32_t                    owPinNum;
    IoPortId                    owPinLetter;
};

struct vtblOneWire {
    int32_t (*powerBus)             (const struct oneWire *self);
    int32_t (*txBit)                (const struct oneWire *self, BIT b);
    int32_t (*txByte)               (const struct oneWire *self, BYTE b);
    int32_t (*tx)                   (const struct oneWire *self,
                                        const void *data, size_t len);
    int32_t (*txWithCrc)            (const struct oneWire *self,
                                        const void *data,
                                        size_t len);
    int32_t (*rxBit)                (const struct oneWire *self);
    int32_t (*rx)                   (const struct oneWire *self, void *dst,
                                        size_t len);
    int32_t (*rxCheckCrc)           (const struct oneWire *self, void *dst,
                                        size_t len);
    int32_t (*reset)                (const struct oneWire *self);
    int32_t (*findDevices)          (struct oneWire *self, union romCode *dst,
                                        size_t dstSizeBytes, BYTE searchRomCmd);
    int32_t (*findFamily)           (struct oneWire *self, union romCode *dst,
                                        size_t dstSizeBytes, BYTE searchRomCmd,
                                        BYTE familyCode);
    int32_t (*findSkipFamily)       (struct oneWire *self, union romCode *dst,
                                        size_t dstSizeBytes, BYTE searchRomCmd,
                                        BYTE familyCode);
    int32_t (*verify)               (struct oneWire *self, union romCode rc,
                                        BYTE searchRomCmd);
};

inline uint32_t
owCrc(const void *data, size_t len);

int32_t
oneWire_new(struct oneWire *self, IoPortId pinLtr, uint32_t pinNum);

#endif
