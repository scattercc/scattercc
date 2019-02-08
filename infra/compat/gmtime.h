#if !defined(_INFRA_COMPAT_GMTIME_H_INCLUDED_)
#define _INFRA_COMPAT_GMTIME_H_INCLUDED_

#include <ctime>

namespace infra
{
    inline bool my_gmtime(std::tm* const stm, const time_t* const stime)
    {
#if HAVE_GMTIME_R
        const std::tm* const ret = gmtime_r(stime, stm);
        return (ret != nullptr);
#elif HAVE_GMTIME_S
        const errno_t err = gmtime_s(stm, stime);
        return (err == 0);
#elif HAVE_GMTIME
        tm* const ptm = gmtime(stime);
        if (ptm == nullptr) return false;

        *stm = std::move(*ptm);
        return true;
#else
#error "None of these are defined true-like: HAVE_GMTIME_R, HAVE_GMTIME_S, HAVE_GMTIME"
#endif
    }

}  // namespace infra

#endif  // !defined(_INFRA_COMPAT_GMTIME_H_INCLUDED_)
