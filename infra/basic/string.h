#if !defined(_INFRA_BASIC_STRING_H_INCLUDED_)
#define _INFRA_BASIC_STRING_H_INCLUDED_

#include <predef/static_assert.h>
#include <predef/types.h>
#include <basic/basic_math.h>

namespace infra
{
    template<typename TCh, size_t _Size>
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
            for (size_t i = 0; i < _Size - 1; ++i)
            {
                value[i] = fill_char;
            }
            write_null_terminator();
        }

        template<size_t _StrSize>
        constexpr int compare(const TCh (&str)[_StrSize]) const noexcept
        {
            INFRA_STATIC_ASSERT(_StrSize > 0);

            const size_t minSize = min(_Size, _StrSize);
            for (size_t i = 0; i < minSize; ++i)
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
            size_t _SrcStart,
            size_t _MyStart,
            size_t _Length,
            size_t _RepeatTimes,
            size_t _SrcSize>
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

            for (size_t time = 0; time < _RepeatTimes; time++)
            {
                for (size_t i = 0; i < _Length; ++i)
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
        constexpr size_t length() const noexcept
        {
            return _Size - 1;
        }

        template<size_t _RepeatTimes>
        constexpr auto repeat() const noexcept
        {
            INFRA_STATIC_ASSERT_NO_OVERFLOW_MUL(_Size - 1, _RepeatTimes);
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD((_Size - 1) * _RepeatTimes, 1);

            string<TCh, (_Size - 1) * _RepeatTimes + 1> result;
            result.template copy<0, 0, _Size - 1, _RepeatTimes, _Size>(value);
            return result;
        }

        /**
         * Searches the string for the first character, which matches ANY of the characters
         * specified in argument "chars".
         * If "start" is specified, the search only includes characters at or after that position,
         * ignoring any possible occurrences before "start".
         * If not found, returns -1.
         */
        template<size_t _NChars>
        constexpr size_t find_first_of(const TCh (&chars)[_NChars], size_t start = 0) const noexcept
        {
            for (size_t pos = start; pos < length(); ++pos)
            {
                for (size_t i = 0; i < length_of_cstr(chars); ++i)
                {
                    if (value[pos] == chars[i]) return pos;
                }
            }
            return (size_t)-1;
        }

        /**
         * Searches the string for the last character, which matches ANY of the characters
         * specified in argument "chars".
         * If "start" is specified, the search only includes characters at or before that position,
         * ignoring any possible occurrences after "start".
         * If not found, returns -1.
         */
        template<size_t _NChars>
        constexpr size_t find_last_of(const TCh (&chars)[_NChars], size_t start = _Size - 1) const noexcept
        {
            for (size_t pos = min(start, length() - 1); pos != (size_t)-1; --pos)
            {
                for (size_t i = 0; i < length_of_cstr(chars); ++i)
                {
                    if (value[pos] == chars[i]) return pos;
                }
            }
            return (size_t)-1;
        }

        /**
         * Return the substring of current string starting from "_Start", with length "_Length"
         */
        template<size_t _Start, size_t _Length>
        constexpr auto sub_string() const noexcept
        {
            // Make sure not overflow!
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Start, _Length);

            INFRA_STATIC_ASSERT(_Start < _Size);
            INFRA_STATIC_ASSERT(_Start + _Length < _Size);

            string<TCh, _Length + 1> result;
            result.template copy<_Start, 0, _Length, 1, _Size>(value);
            return result;
        }

        /**
         * Return the substring of current string starting from "_Start"
         */
        template<size_t _Start>
        constexpr auto sub_string() const noexcept
        {
            INFRA_STATIC_ASSERT(_Start < _Size);

            return sub_string<_Start, (_Size - 1) - _Start>();
        }

        /**
         * Add padding at end of current string with specific character
         */
        template<size_t _Length>
        constexpr auto pad_right(const TCh ch) const noexcept
        {
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Length, 1);
            INFRA_STATIC_ASSERT(_Length + 1 >= _Size);

            string<TCh, _Length + 1> result(ch);
            result.template copy<0, 0, _Size - 1, 1, _Size>(value);
            return result;
        }

        /**
         * Add padding at left of current string with specific character
         */
        template<size_t _Length>
        constexpr auto pad_left(const TCh ch) const noexcept
        {
            INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Length, 1);
            INFRA_STATIC_ASSERT(_Length + 1 >= _Size);

            string<TCh, _Length + 1> result(ch);
            result.template copy<0, _Length + 1 - _Size, _Size - 1, 1, _Size>(value);
            return result;
        }

        /**
         * Returns true if this string is NOT truncated by inner NULL terminator.
         */
        constexpr bool contains_no_null_char() const noexcept
        {
            for (size_t pos = 0; pos < length(); ++pos)
            {
                if (value[pos] == NULL_CHAR) return false;
            }
            return true;
        }

        constexpr const TCh& operator[](const size_t index) const noexcept
        {
            return value[index];
        }

#define _INFRA_DECLARE_STRING_COMPARE(_Op_) \
        template<size_t _StrSize> \
        constexpr bool operator _Op_(const TCh(&str)[_StrSize]) const noexcept \
        { \
            INFRA_STATIC_ASSERT(_StrSize > 0); \
            return compare(str) _Op_ 0; \
        } \
        \
        template<size_t _StrSize> \
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


    template<typename TCh, size_t _Size1, size_t _Size2>
    constexpr auto operator+(string<TCh, _Size1> str1, const TCh(&str2)[_Size2]) noexcept
    {
        // Ensure no overflow
        INFRA_STATIC_ASSERT(_Size2 > 0);
        INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size1 - 1, _Size2 - 1, 1);

        string<TCh, (_Size1 - 1) + (_Size2 - 1) + 1> result;
        result.template copy<0, 0, _Size1 - 1, 1, _Size1>(str1.value);
        result.template copy<0, _Size1 - 1, _Size2 - 1, 1, _Size2>(str2);
        return result;
    }

    template<typename TCh, size_t _Size1, size_t _Size2>
    constexpr auto operator+(string<TCh, _Size1> str1, string<TCh, _Size2> str2) noexcept
    {
        return str1 + str2.value;
    }

    template<typename TCh, size_t _Size1, size_t _Size2>
    constexpr auto operator+(const TCh(&str1)[_Size1], string<TCh, _Size2> str2) noexcept
    {
        // Ensure no overflow
        INFRA_STATIC_ASSERT(_Size2 > 0);
        INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size1 - 1, _Size2 - 1, 1);

        return string<TCh, 1>() + str1 + str2;
    }

    template<typename TCh, size_t _Size>
    constexpr auto operator+(const string<TCh, _Size> str, const TCh ch) noexcept
    {
        // Ensure no overflow
        INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size, 1);

        return str + string<TCh, 2>(ch);
    }

    template<typename TCh, size_t _Size>
    constexpr auto operator+(const TCh ch, const string<TCh, _Size> str) noexcept
    {
        // Ensure no overflow
        INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(_Size, 1);

        return string<TCh, 2>(ch) + str;
    }


    template<size_t _Size>
    constexpr auto make_string(const string<char, _Size> str) noexcept
    {
        return str;
    }

    template<size_t _Size>
    constexpr auto make_string(const char(&str)[_Size]) noexcept
    {
        return string<char, _Size>(str);
    }

    template<size_t _RepeatTimes = 1>
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
