#ifndef NU_ARITH_H
#define NU_ARITH_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include <stdlib.h>

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

/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_UP_ULL(ll,d) \
	({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })

#if defined(BITS_PER_LONG) && BITS_PER_LONG == 32
# define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP_ULL(ll, d)
#else
# define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP(ll,d)
#endif

/* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
#define roundup(x, y) (					\
{							\
	const typeof(y) __y = y;			\
	(((x) + (__y - 1)) / __y) * __y;		\
}							\
)
#define rounddown(x, y) (				\
{							\
	typeof(x) __x = (x);				\
	__x - (__x % (y));				\
}							\
)

/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(x) __x = x;				\
	typeof(divisor) __d = divisor;			\
	(((typeof(x))-1) > 0 ||				\
	 ((typeof(divisor))-1) > 0 || (__x) > 0) ?	\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
}							\
)

/*
 * Multiplies an integer by a fraction, while avoiding unnecessary
 * overflow or loss of precision.
 */
#define mult_frac(x, numer, denom)(			\
{							\
	typeof(x) quot = (x) / (denom);			\
	typeof(x) rem  = (x) % (denom);			\
	(quot * (numer)) + ((rem * (numer)) / (denom));	\
}							\
)


#endif
