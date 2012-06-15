#include "../include/flash.h"

#define CONST_FLASH_SIZE_WORDS (((CONST_FLASH_SIZE_BYTES)-1)>>2)+1

static const uint32_t   CONST_FLASH[CONST_FLASH_SIZE_WORDS];

int32_t
eraseFlash(void)
{
    int32_t     err = 0;
    uint32_t    retryCount;
    for (retryCount = 0, err = -98; retryCount < 3 && err != 0; retryCount++) {
        size_t i;
        err = 0;
        for (i = 0; i < CONST_FLASH_SIZE_WORDS; ++i) {
            NVMWriteWord(&(CONST_FLASH[i]), ~0U);
            if (CONST_FLASH[i] != ~0U) {
                err = -98;
                break;
            }
        }
    }

    return err;
}

inline int32_t
readFlash(void *dst, size_t siz)
{
    memcpy(dst, CONST_FLASH, siz);

    return (siz > CONST_FLASH_SIZE_BYTES) ? -EEXCEEDSFLASHSIZ : 0;
}

int32_t
writeFlash(const void *src, size_t siz)
{
    int32_t err = 0;
    size_t i;
    uint32_t retryCount;

    const uint32_t *srcWords = (const uint32_t *)src;

    if (src == NULL)
        return -ENULPTR;

    if ((err = eraseFlash()) < 0)
        return err;

    for (retryCount = 0, err = -98; retryCount < 3 && err != 0; retryCount++) {
        err = 0;
        for (i = 0; i*4 < siz && i < CONST_FLASH_SIZE_WORDS; ++i) {
            NVMWriteWord(&(CONST_FLASH[i]), srcWords[i]);
            if (CONST_FLASH[i] != srcWords[i]) {
                err = -98;
                break;
            }
        }
    }

    if (err == 0 && siz > CONST_FLASH_SIZE_BYTES)
        err = -EEXCEEDSFLASHSIZ;

    return err;
}
