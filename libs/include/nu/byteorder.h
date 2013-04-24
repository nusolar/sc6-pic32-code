#ifndef NU_BYTEORDER_H
#define NU_BYTEORDER_H 1

/* provide macros/functions:
 * 
 * htobe16 (host to big-endian for 16-bit integers)
 * htobe32 ("    "  "          "   32-bit integers)
 * betoh16 (big-endian to host for 16-bit integers)
 * betoh32 ("          "  "    "   32-bit integers)
 * htole16 (little endian...)
 * htole32
 * letoh16
 * letoh32
 *
 * also includes swap16 and swap32
 */
#include <sys/endian.h>
#include "nu/compiler.h"
#include "nu/nu_types.h"

static ALWAYSINLINE u16
bswap_u16(u16 val)
{
    return (val << 8) | (val >> 8);
}

static ALWAYSINLINE s16
bswap_s16(s16 val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

static ALWAYSINLINE u32
bswap_u32(u32 val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

static ALWAYSINLINE s32
bswap_s32(s32 val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

static ALWAYSINLINE u64 bswap_u64(u64 val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

static ALWAYSINLINE s64 bswap_s64(s64 val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

#endif
