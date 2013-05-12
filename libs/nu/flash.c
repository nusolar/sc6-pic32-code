#include "../include/nu/flash.h"

#define CONST_FLASH_SIZE_WORDS (((CONST_FLASH_SIZE_BYTES)-1)>>2)+1

/* Note that:
 *    "bytes" needs to be a multiple of BYTE_PAGE_SIZE (and aligned that way)
 *      if you intend to erase
 *    "bytes" needs to be a multiple of BYTE_ROW_SIZE (and aligned that way)
 *      if you intend to write rows
 *    "bytes" needs to be a multiple of sizeof(int) if you intend to write words
 */
#define NVM_ALLOCATE(name, align, bytes) volatile unsigned char name[(bytes)] \
  __attribute__((aligned(align),space(prog),section(".nvm"))) = \
  { [0 ... (bytes)-1] = 0xFF }

NVM_ALLOCATE(_flash, BYTE_PAGE_SIZE, BYTE_PAGE_SIZE);

int32_t
eraseFlash(void)
{
    int32_t     err = 0;
    uint32_t    maxAttempts = 3;
    while (maxAttempts--) {
        size_t ui;
        for (ui = 0, err = 0; ui < CONST_FLASH_SIZE_BYTES/BYTE_PAGE_SIZE; ++ui) {
            clear_wdt();
            NVMErasePage((void *)(_flash+ui*BYTE_PAGE_SIZE));
            if (*((volatile uint32_t *)(_flash + ui*BYTE_PAGE_SIZE)) != 0xFFFFFFFF) {
                err = -EOTHER;
                break;
            }
        }
        if (!err)
            break;
    }

    return err;
}

inline int32_t
readFlash(void *dst, size_t siz)
{
    memcpy(dst, _flash, siz);

    return (siz > CONST_FLASH_SIZE_BYTES) ? -EEXCEEDSFLASHSIZ : 0;
}

int32_t
writeFlash(const void *src, size_t siz)
{
    int32_t err = 0;
    size_t ui;
    uint32_t maxAttempts = 3;

    const unsigned int *srcWords = (const unsigned int *)src;

    if (src == NULL)
        return -ENULPTR;

    if ((err = eraseFlash()) < 0)
        return err;

    while (maxAttempts--) {
        clear_wdt();
        for (ui = 0, err = 0; ui*sizeof(int) < siz &&
                ui < CONST_FLASH_SIZE_BYTES/sizeof(int); ++ui) {
            clear_wdt();
            NVMWriteWord(_flash+ui*sizeof(int), srcWords[ui]);
            if (*((volatile uint32_t *)(_flash+ui*sizeof(int))) != srcWords[ui]) {
                err = -EOTHER;
                break;
            }
        }
        if (!err)
            break;
    }

    if (err == 0 && siz > CONST_FLASH_SIZE_BYTES)
        err = -EEXCEEDSFLASHSIZ;

    return err;
}
