#if !defined(_INFRA_COMPAT_GMTIME_H_INCLUDED_)
#define _INFRA_COMPAT_GMTIME_H_INCLUDED_

#include <ctime>
#include <type_traits>
#include <utility>

namespace infra
{
    namespace details
    {
        std::false_type gmtime_s(...);
        std::false_type gmtime_r(...);
        std::false_type gmtime(...);

        static constexpr const bool have_gmtime_s =
            std::is_same<
                decltype(gmtime_s(std::declval<tm*>(), std::declval<const time_t*>())),
                errno_t
            >::value;

        static constexpr const bool have_gmtime_r =
            std::is_same<
                decltype(gmtime_r(std::declval<const time_t*>(), std::declval<tm*>())),
                tm*
            >::value;

        static constexpr const bool have_gmtime =
            std::is_same<
                decltype(gmtime(std::declval<const time_t*>())),
                tm*
            >::value;


        template<bool _Have_gmtime>
        struct my_gmtime_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                tm* const ptm = gmtime(stime);
                if (ptm == nullptr) return false;

                *stm = std::move(*ptm);
                return true;
            }
        };

        template<>
        struct my_gmtime_impl</*_Have_gmtime*/false>
        {
            // Have none of gmtime_r, gmtime_s, gmtime
            // Compilation should fail!
            static bool call(std::tm* const stm, const time_t* const stime);
        };


        template<bool _Have_gmtime_s>
        struct my_gmtime_s_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                const errno_t err = gmtime_s(stm, stime);
                return (err == 0);
            }
        };

        template<>
        struct my_gmtime_s_impl</*_Have_gmtime_s*/false>
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                return my_gmtime_impl<have_gmtime>::call(stm, stime);
            }
        };


        template<bool _Have_gmtime_r>
        struct my_gmtime_r_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                const std::tm* const ret = gmtime_r(stime, stm);
                return (ret != nullptr);
            }
        };

        template<>
        struct my_gmtime_r_impl</*_Have_gmtime_r*/false>
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                return my_gmtime_s_impl<have_gmtime_s>::call(stm, stime);
            }
        };

    }  // namespace details


    inline bool my_gmtime(std::tm* const stm, const time_t* const stime)
    {
        return details::my_gmtime_r_impl<details::have_gmtime_r>::call(stm, stime);
    }

}  // namespace infra

#endif  // !defined(_INFRA_COMPAT_GMTIME_H_INCLUDED_)
