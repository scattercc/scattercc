#if !defined(_INFRA_COMPAT_LOCALTIME_H_INCLUDED_)
#define _INFRA_COMPAT_LOCALTIME_H_INCLUDED_

#include <ctime>
#include <type_traits>
#include <utility>

namespace infra
{
    namespace details
    {
        std::false_type localtime_s(...);
        std::false_type localtime_r(...);
        std::false_type localtime(...);

        static constexpr const bool have_localtime_s =
            !std::is_same<
                decltype(localtime_s(std::declval<tm*>(), std::declval<const time_t*>())),
                std::false_type
            >::value;

        static constexpr const bool have_localtime_r =
            !std::is_same<
                decltype(localtime_r(std::declval<const time_t*>(), std::declval<tm*>())),
                std::false_type
            >::value;

        static constexpr const bool have_localtime =
            !std::is_same<
                decltype(localtime(std::declval<const time_t*>())),
                std::false_type
            >::value;


        template<bool _Have_localtime>
        struct my_localtime_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                tm* const ptm = localtime(stime);
                if (ptm == nullptr) return false;

                *stm = std::move(*ptm);
                return true;
            }
        };

        template<>
        struct my_localtime_impl</*_Have_localtime*/false>
        {
            // Have none of localtime_r, localtime_s, localtime
            // Compilation should fail!
            static bool call(std::tm* const stm, const time_t* const stime);
        };


        template<bool _Have_localtime_s>
        struct my_localtime_s_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                const errno_t err = localtime_s(stm, stime);
                return (err == 0);
            }
        };

        template<>
        struct my_localtime_s_impl</*_Have_localtime_s*/false>
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                return my_localtime_impl<have_localtime>::call(stm, stime);
            }
        };


        template<bool _Have_localtime_r>
        struct my_localtime_r_impl
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                const std::tm* const ret = localtime_r(stime, stm);
                return (ret != nullptr);
            }
        };

        template<>
        struct my_localtime_r_impl</*_Have_localtime_r*/false>
        {
            static bool call(std::tm* const stm, const time_t* const stime)
            {
                return my_localtime_s_impl<have_localtime_s>::call(stm, stime);
            }
        };

    }  // namespace details


    inline bool my_localtime(std::tm* const stm, const time_t* const stime)
    {
        return details::my_localtime_r_impl<details::have_localtime_r>::call(stm, stime);
    }

}  // namespace infra

#endif  // !defined(_INFRA_COMPAT_LOCALTIME_H_INCLUDED_)
