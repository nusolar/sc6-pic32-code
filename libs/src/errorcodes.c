#include "errorcodes.h"
#include "utility/arith.h"

const char *error_names[] = {
#define ERROR(x) #x,
    ERRORS
#undef ERROR
};

/* error_names must contain a string descriptor for each error in the errors enum */
STATIC_ASSERT(NUM_ERRORS == ARRAY_SIZE(error_names),
                ERROR_NAMES_SIZE_MISMATCH_WITH_NUM_ERRORS);

const char *
get_error_name(s32 err)
{
    if ((u16)ABS(err) >= ARRAY_SIZE(error_names))
        return error_names[EOTHER];
    return error_names[ABS(err)];
}
