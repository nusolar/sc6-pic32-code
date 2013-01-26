#ifndef NU_TYPES_H
#define NU_TYPES_H

#include <GenericTypeDefs.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef __bool_true_false_are_defined
typedef enum { false = 0, true } bool;
#endif

typedef BIT bit;
enum { clear = 0, set };

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef u8          byte;

#endif

