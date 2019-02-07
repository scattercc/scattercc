#if !defined(_INFRA_BASIC_PRINTF_H_INCLUDED_)
#define _INFRA_BASIC_PRINTF_H_INCLUDED_

#include "./string.h"
#include <cstdint>
#include <cinttypes>

namespace infra
{
    template<typename T, typename TCh = char>
    struct printf_format : printf_format<raw_type<T>, TCh>
    { };

#define _INFRA_DECLARE_PRINTF_FORMAT(_TCh_, _Type_, _Format_) \
    template<> \
    struct printf_format<_Type_, _TCh_> \
    { \
    private: \
        template<size_type _Size> \
        static constexpr auto to_string(const _TCh_(&str)[_Size]) noexcept \
        { \
            return string<_TCh_, _Size>(str); \
        } \
    public: \
        static constexpr auto format() noexcept \
        { \
            return to_string(_Format_); \
        } \
    }

    _INFRA_DECLARE_PRINTF_FORMAT(char, signed short int, "hd");
    _INFRA_DECLARE_PRINTF_FORMAT(char, signed int, "d");
    _INFRA_DECLARE_PRINTF_FORMAT(char, signed long int, "ld");
    _INFRA_DECLARE_PRINTF_FORMAT(char, signed long long int, "lld");
    _INFRA_DECLARE_PRINTF_FORMAT(char, unsigned short int, "hu");
    _INFRA_DECLARE_PRINTF_FORMAT(char, unsigned int, "u");
    _INFRA_DECLARE_PRINTF_FORMAT(char, unsigned long int, "lu");
    _INFRA_DECLARE_PRINTF_FORMAT(char, unsigned long long int, "llu");

    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, signed short int, INFRA_WSTR("hd"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, signed int, INFRA_WSTR("d"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, signed long int, INFRA_WSTR("ld"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, signed long long int, INFRA_WSTR("lld"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, unsigned short int, INFRA_WSTR("hu"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, unsigned int, INFRA_WSTR("u"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, unsigned long int, INFRA_WSTR("lu"));
    _INFRA_DECLARE_PRINTF_FORMAT(wchar_t, unsigned long long int, INFRA_WSTR("llu"));

#undef _INFRA_DECLARE_PRINTF_FORMAT

    template<typename TCh, size_type _Size, typename T>
    constexpr auto operator+(const string<TCh, _Size> str, const printf_format<T>) noexcept
    {
        return str + printf_format<T>::format();
    }

    template<typename TCh, size_type _Size, typename T>
    constexpr auto operator+(const printf_format<T>, const string<TCh, _Size> str) noexcept
    {
        return printf_format<T>::format() + str;
    }

    template<typename T1, typename T2>
    constexpr auto operator+(const printf_format<T1>, const printf_format<T2>) noexcept
    {
        return printf_format<T1>::format() + printf_format<T2>::format();
    }

    template<typename TCh, size_type _Size, typename T>
    constexpr auto operator+(const TCh (&str)[_Size], const printf_format<T>) noexcept
    {
        return str + printf_format<T>::format();
    }

    template<typename TCh, size_type _Size, typename T>
    constexpr auto operator+(const printf_format<T>, const TCh (&str)[_Size]) noexcept
    {
        return printf_format<T>::format() + str;
    }


}  // namespace infra

#endif  // !defined(_INFRA_BASIC_PRINTF_H_INCLUDED_)
