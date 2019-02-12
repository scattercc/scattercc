#if !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
#define _INFRA_PREDEF_TYPES_H_INCLUDED_

#include "static_assert.h"
#include <cstdint>
#include <array>
#include <type_traits>

namespace infra
{
    typedef std::size_t size_t;
    typedef std::make_signed<size_t>::type ssize_t;

    template<size_t _Value>
    using size_t_constant = std::integral_constant<size_t, _Value>;


    /**
     * Returns whether given type T is a "raw" type, which is, a type with no
     * const and/or volatile modifier(s), nor reference.
     */
    template<typename T>
    struct is_decayed_type
    {
        typedef typename std::is_same<T, std::decay<T>>::value_type value_type;

        static constexpr const value_type value = std::is_same<T, typename std::decay<T>::type>::value;

        constexpr operator value_type() const noexcept { return value; }

        constexpr value_type operator()() const noexcept { return value; }
    };


    template<typename T, size_t _Size>
    constexpr size_t length_of_array(const T(&)[_Size]) noexcept { return _Size; }

    template<typename T, size_t _Size>
    constexpr size_t length_of_array(const std::array<T, _Size>&) noexcept { return _Size; }

    template<typename T, size_t _Size>
    constexpr size_t length_of_cstr(const T(&)[_Size]) noexcept
    {
        static_assert(_Size >= 1, "cstr must contain at least NULL terminator");

        return _Size - 1;
    }

}  // namespace infra

#endif  // !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
