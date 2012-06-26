#ifndef __NU_UTILITY_H
#define __NU_UTILITY_H

#define CLAIM_PIN(n) void claimed_pin_##n(void) {}

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

#define STATIC_ASSERT_HELPER(expr, msg) \
            (!!sizeof (struct { unsigned int STATIC_ASSERTION__##msg: (expr) ? 1 : -1; }))
#define STATIC_ASSERT(expr, msg) \
            extern int (*assert_function__(void)) [STATIC_ASSERT_HELPER(expr, msg)]

#define STATIC_ASSERT_EXISTS(x)     STATIC_ASSERT((x) == (x), x ## _doesnt_exist)

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/*
 * abs() handles unsigned and signed longs, ints, shorts and chars.  For all
 * input types abs() returns a signed long.
 * abs() should not be used for 64-bit types (s64, u64, long long) - use abs64()
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
#undef min
#define min(x, y) ({                            \
        typeof(x) _min1 = (x);                  \
        typeof(y) _min2 = (y);                  \
        (void) (&_min1 == &_min2);              \
        _min1 < _min2 ? _min1 : _min2; })

#undef max
#define max(x, y) ({                            \
        typeof(x) _max1 = (x);                  \
        typeof(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })

#define MIN(x,y)            min(x, y)
#define MAX(x,y)            max(x, y)

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
