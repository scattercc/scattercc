#if !defined(_INFRA_INFRA_H_INCLUDED_)
#define _INFRA_INFRA_H_INCLUDED_

//
// Include target platform dependent headers
//
#if SCC_TARGET_WINDOWS || SCC_TARGET_CYGWIN || SCC_TARGET_MSYS
#   include "mswin/target.h"
#elif SCC_TARGET_LINUX
#   include "posix_base/target.h"
#   include "linux/target.h"
#else
#   error "Unknown target: None of SCC_TARGET_XXX is true-like"
#endif  // ...


#include <predef/ide.h>
#include <predef/maybe_unused.h>
#include <predef/static_assert.h>
#include <predef/types.h>

#include <basic/basic_math.h>
#include <basic/datetime.h>
#include <basic/integer_to_string.h>
#include <basic/logging.h>
#include <basic/printf.h>
#include <basic/string.h>

#endif  // !defined(_INFRA_INFRA_H_INCLUDED_)
