#ifndef NU_UTIL_DATA_H
#define NU_UTIL_DATA_H

#include "static.h"

/* data structure things */

#define member_size(type, member) sizeof(((type *)0)->member)

/* From Linux kernel */
#ifdef offsetof
    #undef  offsetof
#endif
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
                typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})
/* ANSI C Version (sacrifices some type checking) (offsetof() is in <stddef.h>):
 * #define container_of(ptr, type, member) ((type *)((char *)ptr - offsetof(type, member)))
 */

#define ZEROARR(x)          (__must_be_array(x), memset((x), 0, sizeof(x)))
#define CPYARR(dest, src)   \
    (__must_be_array(dest), __must_be_array(src), memcpy((dest), (src), \
    MIN(sizeof(src), sizeof(dest))))
#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#endif
/* #define ARRAY_SIZE(a)       (sizeof(a) / sizeof(*a)) */

#define ZEROVAR(x)          memset(&(x), 0, sizeof(x))

#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#define SWAP(a, b)  swap(a, b)

#endif
