#if !defined(_INFRA_INFRA_H_INCLUDED_)
#define _INFRA_INFRA_H_INCLUDED_

//
// Include target platform dependent headers
//
#if SCC_TARGET_WINDOWS || SCC_TARGET_CYGWIN || SCC_TARGET_MSYS
#   include "mswin/mswin.h"
#else
#   error "Unknown target: None of SCC_TARGET_XXX is true-like"
#endif  // ...


#include "infra/ide.h"
#include "infra/maybe_unused.h"

#endif  // defined(_INFRA_INFRA_H_INCLUDED_)
