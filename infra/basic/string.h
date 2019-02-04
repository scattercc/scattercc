#if !defined(_INFRA_BASIC_STRING_H_INCLUDED_)
#define _INFRA_BASIC_STRING_H_INCLUDED_

#include <predef/static_assert.h>
#include <predef/types.h>
#include <basic/basic_math.h>

namespace infra
{
    template<typename TCh, size_type _Size>
    class string
    {
        static_assert(_Size > 0, "string must contain at least null terminator");

    private:
        constexpr void write_null_terminator() noexcept
        {
            value[_Size - 1] = NULL_CHAR;
        }

        constexpr void fill_with(const TCh fill_char) noexcept
        {
            for (size_type i = 0; i < _Size - 1; ++i)
            {
                value[i] = fill_char;
            }
            write_null_terminator();
        }

        template<size_type _StrSize>
        constexpr int compare(const TCh (&str)[_StrSize]) const noexcept
        {
            INFRA_STATIC_ASSERT(_StrSize > 0);

            const size_type minSize = min(_Size, _StrSize);
            for (size_type i = 0; i < minSize; ++i)
            {
                if (value[i] < str[i]) return -1;
                if (value[i] > str[i]) return 1;
            }
#include <warnings/conditional_expression_is_constant/disable.h>
            if (_Size < _StrSize) return -1;
            if (_Size > _StrSize) return 1;
#include <warnings/conditional_expression_is_constant/restore.h>
            return 0;
        }

    public:
        template<
            size_type _SrcStart,
            size_type _MyStart,
            size_type _Length,
            size_type _RepeatTimes,
            size_type _SrcSize>
        constexpr void copy(const TCh(&src)[_SrcSize]) noexcept
        {
            // Boundary check & make sure no overflow
            INFRA_STATIC_ASSERT_NO_OVERFLOW_MUL(_Length, _RepeatTimes);
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_MyStart, _Length * _RepeatTimes);
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_SrcStart, _Length * _RepeatTimes);

            INFRA_STATIC_ASSERT(_MyStart < _Size);
            INFRA_STATIC_ASSERT(_MyStart + _Length * _RepeatTimes < _Size);

            INFRA_STATIC_ASSERT(_SrcStart < _SrcSize);
            INFRA_STATIC_ASSERT(_SrcStart + _Length < _SrcSize);

            for (size_type time = 0; time < _RepeatTimes; time++)
            {
                for (size_type i = 0; i < _Length; ++i)
                {
                    value[_MyStart + time * _Length + i] = src[_SrcStart + i];
                }
            }
            write_null_terminator();
        }

        constexpr string() noexcept
        {
            fill_with(NULL_CHAR);
        }

        constexpr string(const TCh ch) noexcept
        {
            fill_with(ch);
        }

        constexpr string(const TCh(&str)[_Size]) noexcept
        {
            copy<0, 0, _Size - 1, 1, _Size>(str);
        }

        // ReSharper disable once CppMemberFunctionMayBeStatic
        constexpr size_type length() const noexcept
        {
            return _Size - 1;
        }

        template<size_type _RepeatTimes>
        constexpr auto repeat() const noexcept
        {
            INFRA_STATIC_ASSERT_NO_OVERFLOW_MUL(_Size - 1, _RepeatTimes);
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD((_Size - 1) * _RepeatTimes, 1);

            string<TCh, (_Size - 1) * _RepeatTimes + 1> result;
            result.template copy<0, 0, _Size - 1, _RepeatTimes, _Size>(value);
            return result;
        }

        template<size_type _StrSize>
        constexpr auto operator+(const TCh(&str)[_StrSize]) const noexcept
        {
            // Ensure no overflow
            INFRA_STATIC_ASSERT(_StrSize > 0);
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size - 1, _StrSize - 1, 1);

            string<TCh, (_Size - 1) + (_StrSize - 1) + 1> result;
            result.template copy<0, 0, _Size - 1, 1, _Size>(value);
            result.template copy<0, _Size - 1, _StrSize - 1, 1, _StrSize>(str);
            return result;
        }

        constexpr auto operator+(const TCh ch) const noexcept
        {
            // Ensure no overflow
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size, 1);

            string<TCh, _Size + 1> result;
            result.template copy<0, 0, _Size - 1, 1, _Size>(value);
            result.value[_Size - 1] = ch;
            return result;
        }

        template<size_type _StrSize>
        constexpr auto operator+(string<TCh, _StrSize> str) const noexcept
        {
            return operator+(str.value);
        }

        constexpr const TCh& operator[](const size_type index) const noexcept
        {
            return value[index];
        }

#define _INFRA_DECLARE_STRING_COMPARE(_Op_) \
        template<size_type _StrSize> \
        constexpr bool operator _Op_(const TCh(&str)[_StrSize]) const noexcept \
        { \
            INFRA_STATIC_ASSERT(_StrSize > 0); \
            return compare(str) _Op_ 0; \
        } \
        \
        template<size_type _StrSize> \
        constexpr bool operator _Op_(string<TCh, _StrSize> str) const noexcept \
        { \
            return operator _Op_(str.value); \
        }

        _INFRA_DECLARE_STRING_COMPARE(<)
        _INFRA_DECLARE_STRING_COMPARE(<=)
        _INFRA_DECLARE_STRING_COMPARE(>)
        _INFRA_DECLARE_STRING_COMPARE(>=)
        _INFRA_DECLARE_STRING_COMPARE(==)
        _INFRA_DECLARE_STRING_COMPARE(!=)
#undef _INFRA_DECLARE_STRING_COMPARE

    public:
        static constexpr const TCh NULL_CHAR = TCh(0);

        TCh value[_Size] { };

    };  // class string


    template<size_type _Size>
    constexpr auto make_string(const string<char, _Size> str) noexcept
    {
        return str;
    }

    template<size_type _Size>
    constexpr auto make_string(const char(&str)[_Size]) noexcept
    {
        return string<char, _Size>(str);
    }

    template<size_type _RepeatTimes = 1>
    constexpr auto make_string(const char ch) noexcept
    {
        return string<char, _RepeatTimes + 1>(ch);
    }

    constexpr auto make_string() noexcept
    {
        return make_string("");
    }


}  // namespace infra

#endif  // !defined(_INFRA_BASIC_STRING_H_INCLUDED_)
