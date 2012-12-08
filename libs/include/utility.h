#ifndef __NU_UTILITY_H
#define __NU_UTILITY_H

#include <stdlib.h>
#include "compiler.h"

#define CLAIM_PIN(n)                \
    void claimed_pin_##n(void);     \
    void claimed_pin_##n(void){}

#define BITFIELD_WIDTHMASK(width) \
    ((width) >= 32 ? ~0x0ul : (1ul << ((width) % 32)) - 1)

#define BITFIELD_CAST(x, width) \
    ((x) & BITFIELD_WIDTHMASK(width))

/* From Linux kernel */
#ifdef offsetof
    #undef  offsetof
#endif
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})
/* ANSI C Version (sacrifices some type checking) (offsetof() is in <stddef.h>):
 * #define container_of(ptr, type, member) ((type *)((char *)ptr - offsetof(type, member)))
 */

/*
#define STATIC_ASSERT_HELPER(expr, msg) \
            (!!sizeof (struct { unsigned int STATIC_ASSERTION__##msg: (expr) ? 1 : -1; }))
#define STATIC_ASSERT(expr, msg) \
            extern int (*assert_function__(void)) [STATIC_ASSERT_HELPER(expr, msg)]
 */
#define STATIC_ASSERT_HELPER(COND,MSG) typedef char MSG[(COND)?1:-1]
/* token pasting madness: */
#define STATIC_ASSERT3(X,L,M)   STATIC_ASSERT_HELPER(X,static_assertion_at_line_##L##_##M)
#define STATIC_ASSERT2(X,L,M)   STATIC_ASSERT3(X,L,M)
#define STATIC_ASSERT(X,M)      STATIC_ASSERT2(X,__LINE__,M)

#define STATIC_ASSERT_EXISTS(x)     STATIC_ASSERT((x) == (x), x ## _doesnt_exist)

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

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

union utility_floatInt {
    float       f;
    int32_t     d;
    uint32_t    ud;
};

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#pragma GCC diagnostic ignored "-Wfloat-equal"
static ALWAYSINLINE UNUSED PURE unsigned int
_floatEq(float _x, float _y, int32_t maxUlps)
{
    if (likely(_x == _y)) {
        return 1;
    } else {
        union utility_floatInt x, y;
        int32_t diff;
        x.f = _x;
        if (x.d < 0)
            x.d = (int32_t)(0x80000000 - x.ud);
        y.f = _y;
        if (y.d < 0)
            y.d = (int32_t)(0x80000000 - y.ud);
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

#define ZEROVAR(x)          memset(&(x), 0, sizeof(x))

# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#define ZEROARR(x)          (__must_be_array(x), memset((x), 0, sizeof(x)))
#define CPYARR(dest, src)   \
    (__must_be_array(dest), __must_be_array(src), memcpy((dest), (src), \
    MIN(sizeof(src), sizeof(dest))))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
/* #define ARRAY_SIZE(a)       (sizeof(a) / sizeof(*a)) */
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#define SWAP(a, b)  swap(a, b)

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b)    ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b)  ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b)   ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b)  ((a) & (1<<(b)))

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)

#endif
