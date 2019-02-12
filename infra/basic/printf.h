#if !defined(_INFRA_BASIC_PRINTF_H_INCLUDED_)
#define _INFRA_BASIC_PRINTF_H_INCLUDED_

#include "./string.h"
#include <cstdint>
#include <cinttypes>

namespace infra
{
    template<typename T, typename TCh = char>
    struct printf_format : printf_format<typename std::decay<T>::type, TCh>
    { };

#define _INFRA_DECLARE_PRINTF_FORMAT(_TCh_, _Type_, _Format_, /*template*/...) \
    template<__VA_ARGS__> \
    struct printf_format<_Type_, _TCh_> \
    { \
    private: \
        template<size_t __Size> \
        static constexpr auto to_string(const _TCh_(&str)[__Size]) noexcept \
        { \
            return string<_TCh_, __Size>(str); \
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
    //_INFRA_DECLARE_PRINTF_FORMAT(char, float, "f");
    //_INFRA_DECLARE_PRINTF_FORMAT(char, double, "lf");
    _INFRA_DECLARE_PRINTF_FORMAT(char, char, "c");
    _INFRA_DECLARE_PRINTF_FORMAT(char, char*, "s");
    _INFRA_DECLARE_PRINTF_FORMAT(char, const char*, "s");
    _INFRA_DECLARE_PRINTF_FORMAT(char, wchar_t*, "ls");
    _INFRA_DECLARE_PRINTF_FORMAT(char, std::string, "s");
    _INFRA_DECLARE_PRINTF_FORMAT(char, std::wstring, "ls");
    _INFRA_DECLARE_PRINTF_FORMAT(char, void*, "p");
    _INFRA_DECLARE_PRINTF_FORMAT(char, bool, "s");
    _INFRA_DECLARE_PRINTF_FORMAT(char, JUST(string<char, _Size>), "s", size_t _Size);


    // Add them accordingly if needed
    //_INFRA_DECLARE_PRINTF_FORMAT(wchar_t, signed short int, INFRA_WSTR("hd"));

#undef _INFRA_DECLARE_PRINTF_FORMAT

    template<typename TCh, size_t _Size, typename T>
    constexpr auto operator+(const string<TCh, _Size> str, const printf_format<T>) noexcept
    {
        return str + printf_format<T>::format();
    }

    template<typename TCh, size_t _Size, typename T>
    constexpr auto operator+(const printf_format<T>, const string<TCh, _Size> str) noexcept
    {
        return printf_format<T>::format() + str;
    }

    template<typename T1, typename T2>
    constexpr auto operator+(const printf_format<T1>, const printf_format<T2>) noexcept
    {
        return printf_format<T1>::format() + printf_format<T2>::format();
    }

    template<typename TCh, size_t _Size, typename T>
    constexpr auto operator+(const TCh (&str)[_Size], const printf_format<T>) noexcept
    {
        return str + printf_format<T>::format();
    }

    template<typename TCh, size_t _Size, typename T>
    constexpr auto operator+(const printf_format<T>, const TCh (&str)[_Size]) noexcept
    {
        return printf_format<T>::format() + str;
    }


}  // namespace infra

#endif  // !defined(_INFRA_BASIC_PRINTF_H_INCLUDED_)
