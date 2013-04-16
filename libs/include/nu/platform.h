#ifndef NU_PLATFORM_H
#define NU_PLATFORM_H 1

#define NU_PLATFORM_GENERIC 0
#define NU_PLATFORM_PIC32MX 1
#define NU_PLATFORM_UNKNOWN 2

#ifndef NU_PLATFORM
# if defined(__PIC32MX) || defined(__PIC32MX__)
#  define NU_PLATFORM NU_PLATFORM_PIC32MX
# else
#  define NU_PLATFORM NU_PLATFORM_UNKNOWN
# endif
#endif

#endif

