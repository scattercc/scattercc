#if !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
#define _INFRA_PREDEF_TYPES_H_INCLUDED_

#include "static_assert.h"
#include <cstdint>
#include <array>
#include <type_traits>

namespace infra
{
    typedef std::size_t size_type;

    typedef typename std::make_signed<size_type>::type ssize_type;

    template<size_type _Value>
    using size_type_constant = std::integral_constant<size_type, _Value>;



    template<typename T, size_type _Size>
    constexpr size_type length_of_array(const T(&)[_Size]) noexcept { return _Size; }

    template<typename T, size_type _Size>
    constexpr size_type length_of_array(const std::array<T, _Size>&) noexcept { return _Size; }

    template<typename T, size_type _Size>
    constexpr size_type length_of_cstr(const T(&)[_Size]) noexcept
    {
        static_assert(_Size >= 1, "cstr must contain at least NULL terminator");

        return _Size - 1;
    }


}  // namespace infra

#endif  // !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
