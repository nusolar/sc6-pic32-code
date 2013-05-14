#ifndef NU_COMPILER_H
#define NU_COMPILER_H 1

#ifdef __cplusplus
/* workaround needed to prevent keywords from being defined before
 * C++ header is included. <limits> could actually in theory be any
 * C++ header.
 */
# include <limits>

extern "C" {
#endif

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
#define COMPILE_ERROR(args) __attribute__((error args))

/*
 * Prevent the compiler from merging or refetching accesses.  The compiler
 * is also forbidden from reordering successive instances of ACCESS_ONCE(),
 * but only when the compiler is aware of some particular ordering.  One way
 * to make the compiler aware of ordering is to put the two invocations of
 * ACCESS_ONCE() in different C statements.
 *
 * This macro does absolutely -nothing- to prevent the CPU from reordering,
 * merging, or refetching absolutely anything at any time.  Its main intended
 * use is to mediate communication between process-level code and irq/NMI
 * handlers, all running on the same CPU.
 */
#ifndef ACCESS_ONCE
# define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
#endif

#define unreachable() __builtin_unreachable()

#ifdef __cplusplus
} /* extern "C" */

template<typename T>
static ALWAYSINLINE T volatile &access_once(T &t) {
    return static_cast<T volatile &>(t);
}
# undef ACCESS_ONCE
# define ACCESS_ONCE(x) access_once(x)
#endif

#endif
