#ifndef NUPP_UTIL_BITS_HPP
#define NUPP_UTIL_BITS_HPP 1

#define BITFIELD_WIDTHMASK(width) \
    ((width) >= 32 ? ~0x0ul : (1ul << ((width) % 32)) - 1)

#define BITFIELD_CAST(x, width) \
    ((x) & BITFIELD_WIDTHMASK(width))

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

#endif
