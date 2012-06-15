#ifndef __NUSPI_H
#define __NUSPI_H

#include <plib.h>
#include "NU32v2.h"
#include "../include/Utility.h"

/* chip select pin - CHANGE IT HERE */
#if !(defined SPI_CS_PIN_LTR && defined SPI_CS_PIN_NUM)
    /* G9: pic32mx795f512l pin 14; pin 7 on the NU32v2;
     * PORTG is a bidirectional I/O port
     */
    #define SPI_CS_PIN_LTR	G
    #define SPI_CS_PIN_NUM	9
#endif

/* eg. G9 */
#define SPI_CS_PIN	PPCAT(SPI_CS_PIN_LTR, SPI_CS_PIN_NUM)
/* eg. LATGbits.LATG9 */
#define SPI_CS_LAT	PPCAT(PPCAT(LAT, SPI_CS_PIN_LTR), PPCAT(bits.LAT, SPI_CS_PIN))
/* eg. LATGbits.LATG9 (equivalent to SPI_CS_LAT) */
#define SPI_CS		SPI_CS_LAT
/* eg. TRISGbits.TRISG9 */
#define SPI_CS_TRIS PPCAT(PPCAT(TRIS, SPI_CS_PIN_LTR), PPCAT(bits.TRIS, SPI_CS_PIN))

#define DESIRED_BITRATE 1000000 /* 1Mb/s */

void
NUSPITx(SpiChannel chn, const BYTE *data, size_t len);

void
NUSPIRx(SpiChannel chn, BYTE *data, size_t len);

/* initialize SPI: set SPI_CS as output, call SpiChnOpen(), and drive SPI_CS high */
inline void
initSPI(SpiChannel chn) __attribute__((always_inline));

#endif
