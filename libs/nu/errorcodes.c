#include "nu/errorcodes.h"
#include "nu/utility/static.h"
#include "nu/utility/data.h"
#include <stdlib.h>

#define NU_ERROR_NAMES(x) #x,
const char *nu_error_names[] = {
    NU_ERRORS(NU_ERROR_NAMES)
};

#undef __must_be_array
#define __must_be_array(a) 0

/* error_names must contain a string descriptor for each error in the errors enum */
STATIC_ASSERT(NUM_ERRORS == ARRAY_SIZE(nu_error_names),
                ERROR_NAMES_SIZE_MISMATCH_WITH_NUM_ERRORS);

const char *
nu_error_get_name(s32 err)
{
    err = (u16)abs(err);
    if ((u16)err >= ARRAY_SIZE(nu_error_names))
        return nu_error_names[EOTHER];
    return nu_error_names[err];
}
