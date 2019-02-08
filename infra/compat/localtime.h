#if !defined(_INFRA_COMPAT_LOCALTIME_H_INCLUDED_)
#define _INFRA_COMPAT_LOCALTIME_H_INCLUDED_

#include <ctime>

namespace infra
{
    inline bool my_localtime(std::tm* const stm, const time_t* const stime)
    {
#if HAVE_LOCALTIME_R
        const std::tm* const ret = localtime_r(stime, stm);
        return (ret != nullptr);
#elif HAVE_LOCALTIME_S
        const errno_t err = localtime_s(stm, stime);
        return (err == 0);
#elif HAVE_LOCALTIME
        tm* const ptm = localtime(stime);
        if (ptm == nullptr) return false;

        *stm = std::move(*ptm);
        return true;
#else
#error "None of these are defined true-like: HAVE_LOCALTIME_R, HAVE_LOCALTIME_S, HAVE_LOCALTIME"
#endif
    }

}  // namespace infra

#endif  // !defined(_INFRA_COMPAT_LOCALTIME_H_INCLUDED_)
