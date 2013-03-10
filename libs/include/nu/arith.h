#ifndef NU_ARITH_H
#define NU_ARITH_H

#include "compiler.h"
#include "nu_types.h"
#include "utility.h"

/*
 * ABS() handles unsigned and signed longs, ints, shorts and chars.  For all
 * input types ABS() returns a signed long.
 * ABS() should not be used for 64-bit types (s64, u64, long long) - use ABS64()
 * for those.
 */
#define ABS(x) ({                                               \
                long ret;                                       \
                if (sizeof(x) == sizeof(long)) {                \
                        long __x = (x);                         \
                        ret = (__x < 0) ? -__x : __x;           \
                } else {                                        \
                        int __x = (x);                          \
                        ret = (__x < 0) ? -__x : __x;           \
                }                                               \
                ret;                                            \
        })

#define ABS64(x) ({                             \
                s64 __x = (x);                  \
                (__x < 0) ? -__x : __x;         \
        })

/* min and max macros do strict type checking via the pointer comparison */
#define MIN(x, y) ({                            \
        typeof(x) _min1 = (x);                  \
        typeof(y) _min2 = (y);                  \
        (void) (&_min1 == &_min2);              \
        _min1 < _min2 ? _min1 : _min2; })

#define MAX(x, y) ({                            \
        typeof(x) _max1 = (x);                  \
        typeof(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })

#define __HALF_MAX_SIGNED(type) ((type)1 << (sizeof(type)*8-2))
#define __MAX_SIGNED(type) (__HALF_MAX_SIGNED(type) - 1 + __HALF_MAX_SIGNED(type))
#define __MIN_SIGNED(type) (-1 - __MAX_SIGNED(type))

/* return min value type can hold */
#define __MIN(type) ((type)-1 < 1?__MIN_SIGNED(type):(type)0)
/* return max value type can hold */
#define __MAX(type) ((type)~__MIN(type))

/**
 * clamp - return a value clamped to a given range with strict typechecking
 * @val: current value
 * @min: minimum allowable value
 * @max: maximum allowable value
 *
 * This macro does strict typechecking of min/max to make sure they are of the
 * same type as val.  See the unnecessary pointer comparisons.
 */
#define clamp(val, min, max) ({                 \
        typeof(val) __val = (val);              \
        typeof(min) __min = (min);              \
        typeof(max) __max = (max);              \
        (void) (&__val == &__min);              \
        (void) (&__val == &__max);              \
        __val = __val < __min ? __min: __val;   \
        __val > __max ? __max: __val; })

union utility_floatInt {
    float f;
    s32 d;
    u32 ud;
};

#pragma GCC diagnostic ignored "-Wfloat-equal"
static ALWAYSINLINE UNUSED PURE unsigned int
_floatEq(float _x, float _y, s32 maxUlps)
{
    if (likely(_x == _y)) {
        return 1;
    } else {
        union utility_floatInt x, y;
        s32 diff;
        x.f = _x;
        if (x.d < 0)
            x.d = (s32)(0x80000000 - x.ud);
        y.f = _y;
        if (y.d < 0)
            y.d = (s32)(0x80000000 - y.ud);
        diff = abs(x.d-y.d);
        if (diff <= maxUlps)
            return 1;
        return 0;
    }
}

#pragma GCC diagnostic warning "-Wfloat-equal"
/* maxUlps is the max number of floating point numbers between x and y
 * 5 may be a good value for it...
 */
#define FLOATEQ(x, y, maxUlps)                                                      \
    ({                                                                              \
        STATIC_ASSERT((maxUlps) > 0 && (maxUlps) < 4*1024*1024, INVALID_MAXULPS);   \
        _floatEq((x), (y), (maxUlps));                                              \
    })

/* shorthand */
#define FEQ(x, y)   FLOATEQ((x), (y), 4)

#endif

