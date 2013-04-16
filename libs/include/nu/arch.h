#ifndef NU_ARCH_H
#define NU_ARCH_H 1

#define NU_ARCH_PIC32MX 0
#define NU_ARCH_UNKNOWN 1

#if defined(__PIC32MX) || defined(__PIC32MX__)
# define NU_ARCH NU_ARCH_PIC32MX
#else
# define NU_ARCH NU_ARCH_UNKNOWN
#endif

#endif

