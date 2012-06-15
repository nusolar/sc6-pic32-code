#ifndef __NU_FLASH_H
#define __NU_FLASH_H

#include <plib.h>
#include <stdint.h>
#include <string.h>
#include "errorcodes.h"

#define CONST_FLASH_SIZE_BYTES 24

int32_t
eraseFlash(void);

inline int32_t
readFlash(void *dst, size_t siz);

int32_t
writeFlash(const void *src, size_t siz);

#endif
