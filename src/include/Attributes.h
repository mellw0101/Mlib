#pragma once

#define __attr(...)                   __attribute__((__VA_ARGS__))
#define __warn_unused                 __attr(__warn_unused_result__)
#define __no_return                   __attr(__noreturn__)
#define __no_throw                    __attr(__nothrow__)
#define __no_debug                    __attr(__nodebug__)
#define __pure                        __attr(__pure__)
#define __align_size(size)            __attr(__aligned__(size))
#define __no_null(...)                __attr(__nonnull__(__VA_ARGS__))
#define __pnotnull                    __attr(nonnull)

#define __visibility(V)               __attr((__visibility__(#V)))
#define __export                      __visibility(default)
#define __hidden                      __visibility(hidden)

#define __type_visibility(V)          __attr(__type_visibility__(#V))
#define __type_export                 __type_visibility(default)
#define __type_hidden                 __type_visibility(hidden)

#define __diagnose_if(cond, msg, lvl) __attr(__diagnose_if__(cond, msg, lvl))
#define __error_if(cond, msg)         __attr(__diagnose_if__(cond, msg, "error"))
#define __warning_if(cond, msg)       __attr(__diagnose_if__(cond, msg, "warning"))

#define __exclude_from_explicit_instantiation \
	__attribute__((__exclude_from_explicit_instantiation__))
