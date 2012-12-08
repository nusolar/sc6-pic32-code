#include "../include/errorcodes.h"

#define X(a) #a,
const char *error_names[] = {
    ERRORS
};
#undef X

/* error_names must contain a string descriptor for each error in the errors enum */
STATIC_ASSERT(NUM_ERRORS == ARRAY_SIZE(error_names),
                ERROR_NAMES_SIZE_MISMATCH_WITH_NUM_ERRORS);

const char *
getErrorName(int32_t err)
{
    if ((uint16_t)ABS(err) >= ARRAY_SIZE(error_names))
        return error_names[EOTHER];
    else
        return error_names[ABS(err)];
}
