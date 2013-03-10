#ifndef NU_UTIL_STATIC_H
#define NU_UTIL_STATIC_H

/* static (compile-time) checks */

/*
#define STATIC_ASSERT_HELPER(expr, msg) \
            (!!sizeof (struct { unsigned int STATIC_ASSERTION__##msg: (expr) ? 1 : -1; }))
#define STATIC_ASSERT(expr, msg) \
            extern int (*assert_function__(void)) [STATIC_ASSERT_HELPER(expr, msg)]
 */

#define STATIC_ASSERT_HELPER(COND,MSG) typedef char MSG[(COND)?1:-1]
/* token pasting madness: */
#define STATIC_ASSERT3(X,L,M)   STATIC_ASSERT_HELPER(X,static_assertion_at_line_##L##_##M)
#define STATIC_ASSERT2(X,L,M)   STATIC_ASSERT3(X,L,M)
#define STATIC_ASSERT(X,M)      STATIC_ASSERT2(X,__LINE__,M)

#define STATIC_ASSERT_EXISTS(x)     STATIC_ASSERT((x) == (x), x ## _doesnt_exist)

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#endif
