#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu/platform/param.h"
#include "nu/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 nu__Param__Hz_t;

extern nu__Param__Hz_t nu__Param__Hz;
#define NU_HZ   nu__Param__Hz
#define NU_MHZ  (NU_HZ/1000000UL)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
