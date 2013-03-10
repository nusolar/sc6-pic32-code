#ifndef NU_COMPILER_H
#define NU_COMPILER_H 1

/**
 * @file
 * Compiler-specific keywords
 *
 * These can be conditionally redefined if compiling with something other than
 * GCC so that everything still compiles.
 */

#define inline          __inline__
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#define NOINLINE        __attribute__((noinline))
#if !defined(__DEBUG) || defined(NDEBUG)
# define INLINE          inline
# define ALWAYSINLINE    inline  __attribute__((always_inline))
#else
# define INLINE
# define ALWAYSINLINE
#endif

#define ASM             __asm__
#define VOLATILE        __volatile__

#define PACKED          __attribute__((packed))
#define ALIGNED(x)      __attribute__((aligned(x)))
#define CONST           __attribute__((const))
#define NORETURN        __attribute__((noreturn))
#define PURE            __attribute__((pure))
#define PRINTF(a, b)    __attribute__((format(printf, a, b)))
#define MUST_CHECK      __attribute__((warn_unused_result))
#define HOT             __attribute__((hot))
#define COLD            __attribute__((cold))
#define UNUSED          __attribute__((unused))
#define MAYBE_UNUSED    UNUSED
/* i.e. NONNULL((2, 3)) */
#define NONNULL(args)   __attribute__((nonnull args))
#define DEPRECATED      __attribute__((deprecated))
#define ALIAS(a)        __attribute__((alias(a)))
#define WEAK            __attribute__((weak))
/* execute before main() */
#define CONSTRUCTOR(args)   __attribute__((constructor args))

#endif
