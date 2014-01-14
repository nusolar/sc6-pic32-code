#ifndef NU_FLASH_H
#define NU_FLASH_H 1

#include "nu/wdt.h"
#include "nu/utility/static.h"
#include "nu/errorcodes.h"
#include "nu/compiler.h"
#include <plib.h>
#include <stdint.h>
#include <string.h>

#define CONST_FLASH_SIZE_BYTES BYTE_PAGE_SIZE

STATIC_ASSERT(!(CONST_FLASH_SIZE_BYTES%BYTE_PAGE_SIZE),
        FLASH_SIZE_MUST_BE_MULTIPLE_OF_BYTE_PAGE_SIZE);

int32_t
nu__Flash__erase_flash(void);

INLINE int32_t
nu__Flash__read_flash(void *dst, size_t siz);

int32_t
nu__Flash__write_flash(const void *src, size_t siz);

#endif
