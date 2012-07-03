#include "../include/ds18x20.h"

const BYTE          DS18B20_FAMILY_CODE = 0x28;
const BYTE          DS18S20_FAMILY_CODE = 0x10;
const union romCode ALL_DEVICES         = {0};
const union romCode ALARM_DEVICES       = {
    .byteArr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};

enum ds18x20RomCmds {
    DS_SEARCH_ROM       = 0xF0,
    DS_READ_ROM         = 0x33,
    DS_MATCH_ROM        = 0x55,
    DS_SKIP_ROM         = 0xCC,
    DS_ALARM_SEARCH     = 0xEC,
};

enum ds18x20FunctionCmds {
    DS_CONVERT_T        = 0x44,
    DS_WRITE_SCRATCH    = 0x4E,
    DS_READ_SCRATCH     = 0xBE,
    DS_COPY_SCRATCH     = 0x48,
    DS_RECALL_E2        = 0xB8,
    DS_READ_POWER       = 0xB4,
};

static int32_t
findDevices (struct ds18x20 *self, union romCode *dst,
                size_t dstSizeBytes);

static int32_t
readScratchpad(struct ds18x20 *self, union dsScratchPad *dst,
                union romCode rc);

static int32_t
readRom(const struct ds18x20 *self, union romCode *dst);

static int32_t
startTempConversion(struct ds18x20 *self, union romCode rc);

static int32_t
alarmSearch (struct ds18x20 *self, union romCode *dst, size_t dstSizeBytes);

static int32_t
writeScratchpad(struct ds18x20 *self, union dsScratchPad sp,
                    union romCode rc);

static int32_t
scratchpadToEeprom(struct ds18x20 *self, union romCode rc);

static inline int32_t
eepromToScratchpad(struct ds18x20 *self, union romCode rc);

static int32_t
readPowerSupply(struct ds18x20 *self, union romCode rc);

static int32_t
pollBusyStatus(struct ds18x20 *self);

static double
readTemp(struct ds18x20 *self, union romCode rc);

static int32_t
verify(struct ds18x20 *self, union romCode rc);

const struct vtbl_ds18b20 ds18b20_ops = {
    .verify                 = &verify,
    .pollBusyStatus         = &pollBusyStatus,
    .readRom                = &readRom,
    .findDevices            = &findDevices,
    .findAlarmDevices       = &alarmSearch,
    .startTempConversion    = &startTempConversion,
    .readTemp               = &readTemp,
    .writeScratch           = &writeScratchpad,
    .readScratch            = &readScratchpad,
    .copyScratch            = &scratchpadToEeprom,
    .recall_e2              = &eepromToScratchpad,
    .readPowerSupply        = &readPowerSupply,
    .setLimits              = NULL,
};

static int32_t
initSequence(const struct ds18x20 *self)
{
    if (!self)
        return -ENULPTR;

    return self->ow.op->reset(&(self->ow));
}

static int32_t
ds18x20_init(const struct ds18x20 *self)
{
    if (!self)
        return -ENULPTR;

    return initSequence(self);
}

int32_t
ds18x20_new(struct ds18x20 *self, IoPortId pinLtr, unsigned int pinNum,
                enum dsParasiticPower parasiticPower)
{
    int32_t err;

    if (!self)
        return -ENULPTR;

    self->op                    = &ds18b20_ops;
    self->parasiticPower        = parasiticPower;
    if ((err = oneWire_new(&(self->ow), pinLtr, pinNum)) < 0)
        return err;

    return ds18x20_init(self);
}

static int32_t
txCmd(const struct ds18x20 *self, BYTE cmd)
{
    if (!self)
        return -ENULPTR;

    return self->ow.op->txByte(&(self->ow), cmd);
}

static int32_t
txRomCmd(const struct ds18x20 *self, enum ds18x20RomCmds cmd)
{
    int32_t err = 0;

    if (!self)
        return -ENULPTR;

    if ((err = initSequence(self)) < 0)
        return err;

    if ((err = txCmd(self, cmd)) < 0)
        return err;

    return 0;
}

static int32_t
readRom(const struct ds18x20 *self, union romCode *dst)
{
    int32_t err = 0;
    const struct oneWire *owp;

    if (!self || !dst)
        return -ENULPTR;

    if ((err = txRomCmd(self, DS_READ_ROM)) < 0)
        return err;

    owp = &(self->ow);

    memset(dst, 0, sizeof(*dst));
    if ((err = owp->op->rxCheckCrc(owp, dst, sizeof(*dst))))
        return err;

    return 0;
}

static int32_t
matchRom(const struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0;
    const struct oneWire *owp;

    if (!self)
        return -ENULPTR;

    if ((err = txRomCmd(self, DS_MATCH_ROM)) < 0)
        return err;

    owp = &(self->ow);
    if ((err = owp->op->txWithCrc(owp, &rc, sizeof(rc))) < 0)
        return err;

    return 0;
}

static int32_t
findDevices (struct ds18x20 *self, union romCode *dst,
                size_t dstSizeBytes)
{
    if (!self || (!dst && dstSizeBytes != 0))
        return -ENULPTR;
    
    return self->ow.op->findDevices(&(self->ow), dst, dstSizeBytes,
            DS_SEARCH_ROM);
}

static int32_t
alarmSearch (struct ds18x20 *self, union romCode *dst,
                size_t dstSizeBytes)
{
    if (!self || (!dst && dstSizeBytes != 0))
        return -ENULPTR;

    return self->ow.op->findDevices(&(self->ow), dst, dstSizeBytes,
            DS_ALARM_SEARCH);
}

static int32_t
skipRom(const struct ds18x20 *self)
{
    if (!self)
        return -ENULPTR;

    return txRomCmd(self, DS_SKIP_ROM);
}

static int32_t
romcodeEquals(union romCode rc1, union romCode rc2)
{
    uint32_t ui;
    for (ui = 0; ui < ARRAY_SIZE(rc1.byteArr); ui++) {
        if (rc1.byteArr[ui] != rc2.byteArr[ui])
            return 0;
    }

    return 1;
}

static int32_t
addressDevice(struct ds18x20 *self, union romCode rc)
{
    if (!self)
        return -ENULPTR;

    if (romcodeEquals(rc, ALL_DEVICES))
        return skipRom(self);
    else if (romcodeEquals(rc, ALARM_DEVICES))
        return alarmSearch(self, NULL, 0);
    else
        return matchRom(self, rc);
}

static int32_t
verify(struct ds18x20 *self, union romCode rc)
{
    if (!self)
        return -ENULPTR;

    if (romcodeEquals(rc, ALL_DEVICES) || romcodeEquals(rc, ALARM_DEVICES))
        return -EINVAL;

    return self->ow.op->verify(&(self->ow), rc, DS_SEARCH_ROM);
}

static int32_t
startTempConversion(struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0;

    if (!self)
        return -ENULPTR;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_CONVERT_T)) < 0)
        return err;

    if (self->parasiticPower == PARASITIC_POWER_ENABLE)
        if ((err = self->ow.op->powerBus(&(self->ow))) < 0)
            return err;

    return 0;
}

static int32_t
pollBusyStatus(struct ds18x20 *self)
{
    int32_t rxBitResult;

    if (!self)
        return -ENULPTR;

    if (self->parasiticPower == PARASITIC_POWER_ENABLE)
        return -EINVALIDOP;

    rxBitResult = self->ow.op->rxBit(&(self->ow));

    if (rxBitResult == 0)
        return DEVICE_BUSY;
    else if (rxBitResult == 1)
        return DEVICE_IDLE;
    else
        return rxBitResult;
}

static int32_t
scratchpadToEeprom(struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0;

    if (!self)
        return -ENULPTR;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_COPY_SCRATCH)) < 0)
        return err;

    if (self->parasiticPower == PARASITIC_POWER_ENABLE)
        if ((err = self->ow.op->powerBus(&(self->ow))) < 0)
            return err;

    return 0;
}

static int32_t
writeScratchpad(struct ds18x20 *self, union dsScratchPad sp,
                    union romCode rc)
{
    int32_t err = 0;
    const struct oneWire *owp;

    if (!self)
        return -ENULPTR;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_WRITE_SCRATCH)) < 0)
        return err;

    owp = &(self->ow);
    if ((err = owp->op->txWithCrc(owp, &sp, sizeof(sp))) < 0)
        return err;

    return 0;
}

static int32_t
readScratchpad(struct ds18x20 *self, union dsScratchPad *dst,
                union romCode rc)
{
    int32_t err;
    const struct oneWire *owp;

    if (!self || !dst)
        return -ENULPTR;

    if (romcodeEquals(rc, ALL_DEVICES) || romcodeEquals(rc, ALARM_DEVICES))
        return -EINVALIDOP;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_READ_SCRATCH)) < 0)
        return err;

    owp = &(self->ow);
    if ((err = owp->op->rxCheckCrc(owp, dst, sizeof(*dst))) < 0)
        return err;

    return 0;
}

static int32_t
recallEeprom(struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0;

    if (!self)
        return -ENULPTR;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_RECALL_E2)) < 0)
        return err;

    return 0;
}

static inline int32_t
eepromToScratchpad(struct ds18x20 *self, union romCode rc)
{
    if (!self)
        return -ENULPTR;

    return recallEeprom(self, rc);
}

static int32_t
readPowerSupply(struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0, rxBitResult;

    if (!self)
        return -ENULPTR;

    if ((err = addressDevice(self, rc)) < 0)
        return err;

    if ((err = txCmd(self, DS_READ_POWER)) < 0)
        return err;

    rxBitResult = self->ow.op->rxBit(&(self->ow));

    if (rxBitResult == 0)
        return PARASITIC_POWER_ENABLE;
    else if (rxBitResult == 1)
        return PARASITIC_POWER_DISABLE;
    else
        return rxBitResult;
}

static inline double
rawTempToDegreesC(int16_t rawTemp)
{
    return (double)rawTemp/(1<<4);
}

static inline int16_t
degreesCToRawTemp(double tempDegreesC)
{
    return (int16_t)(tempDegreesC*(1<<4));
}

static double
readTemp(struct ds18x20 *self, union romCode rc)
{
    int32_t err = 0;
    union dsScratchPad scratch = {0};

    if (!self)
        return -ENULPTR;

    if ((err = readScratchpad(self, &scratch, rc)) < 0)
        return (double)err;
    
    if (rc.familyCode == DS18B20_FAMILY_CODE) {
        return rawTempToDegreesC(scratch.ds18b20Scratch.rawTemp.rawTemp);
    } else if (rc.familyCode == DS18S20_FAMILY_CODE) {
        BIT_CLEAR(scratch.ds18s20Scratch.rawTemp.rawTemp, 0);
        return (double)scratch.ds18s20Scratch.rawTemp.rawTemp/2-0.25+
                ((double)scratch.ds18s20Scratch.countPerDegreeC-(double)scratch.ds18s20Scratch.countRemain) /
                (double)scratch.ds18s20Scratch.countPerDegreeC;
    } else {
        return -EINVALIDOP;
    }
}
