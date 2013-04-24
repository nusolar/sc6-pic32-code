#ifndef NU_FLASH_H
#define NU_FLASH_H 1

#include <plib.h>
#include <stdint.h>
#include <string.h>
#include "nu/errorcodes.h"
#include "nu/wdt.h"

#define CONST_FLASH_SIZE_BYTES BYTE_PAGE_SIZE

STATIC_ASSERT(!(CONST_FLASH_SIZE_BYTES%BYTE_PAGE_SIZE),
        FLASH_SIZE_MUST_BE_MULTIPLE_OF_BYTE_PAGE_SIZE);

int32_t
eraseFlash(void);

inline int32_t
readFlash(void *dst, size_t siz);

int32_t
writeFlash(const void *src, size_t siz);

#endif
