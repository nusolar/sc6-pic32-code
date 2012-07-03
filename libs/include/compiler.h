#ifndef NU_COMPILER_H
#define NU_COMPILER_H

#ifndef __DEBUG
#define INLINE          inline
#define ALWAYSINLINE    inline  __attribute__((always_inline))
#else
#define INLINE
#define ALWAYSINLINE
#endif

#define CONST           __attribute__((const))
#define NORETURN        __attribute__((noreturn))
#define PURE            __attribute__((pure))
#define UNUSED          __attribute__((unused))

#endif