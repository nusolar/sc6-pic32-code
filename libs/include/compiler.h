#ifndef NU_COMPILER_H
#define NU_COMPILER_H

#define inline __inline__

#define NOLINE          __attribute__((noinline))
#ifndef __DEBUG
#define INLINE          inline
#define ALWAYSINLINE    inline  __attribute__((always_inline))
#else
#define INLINE
#define ALWAYSINLINE
#endif

#define ASM             __asm__
#define VOLATILE        __volatile__

#define PACKED          __attribute__((packed))
#define CONST           __attribute__((const))
#define NORETURN        __attribute__((noreturn))
#define PURE            __attribute__((pure))
#define PRINTF(a, b)    __attribute__((format(printf, a, b)))
#define MUST_CHECK      __attribute__((warn_unused_result))
#define HOT             __attribute__((hot))
#define COLD            __attribute__((cold))
#define UNUSED          __attribute__((unused))
#define MAYBE_UNUSED    UNUSED
#define NONNULL(...)    __attribute__((nonnull(__VA_ARGS__)))
#define DEPRECATED      __attribute__((deprecated))
#define ALIAS(a)        __attribute__((alias(a)))
#define WEAK            __attribute__((weak))
/* execute before main() */
#define CONSTRUCTOR(...)    \
                        __attribute__((constructor(__VA_ARGS__)))

#endif
