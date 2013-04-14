#ifndef NU_PARAM_H
#define NU_PARAM_H 1

unsigned long nu_hz = 80000000UL;

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz/((uint32_t)(1 << OSCCONbits.PBDIV)))

#define HZ (80000000UL)

#endif
