#ifndef __NU_UTILITY_H
#define __NU_UTILITY_H

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

#define ABS(my_val) ((my_val) < 0) ? -(my_val) : (my_val)
#define MIN(x,y)            ((y) < (x) ? (y) : (x))
#define MAX(x,y)            ((y) > (x) ? (y) : (x))
#define CPYARR(dest, src)   memcpy((dest), (src), MIN(sizeof(src), sizeof(dest)))
#define ZEROVAR(x)          memset(&(x), 0, sizeof(x))
#define ZEROARR(x)          memset((x), 0, sizeof(x))
#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(*a))
#define SWAP(a, b)          do { a ^= b; b ^= a; a ^= b; } while (0)

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
