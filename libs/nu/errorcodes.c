#include "nu/errorcodes.h"
#include "nu/utility/static.h"
#include "nu/utility/data.h"
#include <stdlib.h>

const char *nu__Error__names[] = {
    NU_ERRORS(NU_ERROR_NAMES)
};

#undef __must_be_array
#define __must_be_array(a) 0

/* error_names must contain a string descriptor for each error in the errors enum */
STATIC_ASSERT(NUM_ERRORS == ARRAY_SIZE(nu__Error__names),
                ERROR_NAMES_SIZE_MISMATCH_WITH_NUM_ERRORS);

const char *
nu__Error__get_name(s32 err)
{
    err = (u16)abs(err);
    if ((u16)err >= ARRAY_SIZE(nu__Error__names))
        return nu__Error__names[EOTHER];
    return nu__Error__names[err];
}
