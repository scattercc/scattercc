#if !defined(_INFRA_BASIC_BASIC_MATH_H_INCLUDED_)
#define _INFRA_BASIC_BASIC_MATH_H_INCLUDED_

#include <predef/static_assert.h>
#include <cstdint>

namespace infra
{
    template<typename T>
    constexpr T min(const T a, const T b) noexcept
    {
        INFRA_STATIC_ASSERT(std::is_literal_type<T>::value);
        return a < b ? a : b;
    }

    template<typename T>
    constexpr T max(const T a, const T b) noexcept
    {
        INFRA_STATIC_ASSERT(std::is_literal_type<T>::value);
        return a < b ? b : a;
    }


    template<size_t _Value1, size_t... _Others>
    struct check_overflow_add
    {
    private:
        typedef check_overflow_add<_Others...> remain;

    public:
#include <warnings/integral_overflow/disable.h>
        static constexpr const size_t result = _Value1 + remain::result;
        static constexpr const bool overflow =
            (remain::overflow) ||
            (_Value1 + remain::result < max(_Value1, remain::result));
#include <warnings/integral_overflow/restore.h>
    };

    template<size_t _Value>
    struct check_overflow_add<_Value>
    {
        static constexpr const size_t result = _Value;
        static constexpr const bool overflow = false;
    };


    template<size_t _Value1, size_t... _Others>
    struct check_overflow_mul
    {
    private:
        typedef check_overflow_mul<_Others...> remain;

    public:
#include <warnings/integral_overflow/disable.h>
        static constexpr const size_t result = _Value1 * remain::result;
        static constexpr const bool overflow =
            (remain::overflow) ||
            (_Value1 != 0 &&
                remain::result != 0 &&
                _Value1 * remain::result < max(_Value1, remain::result));
#include <warnings/integral_overflow/restore.h>
    };

    template<size_t _Value>
    struct check_overflow_mul<_Value>
    {
        static constexpr const size_t result = _Value;
        static constexpr const bool overflow = false;
    };

#define INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(...) \
    INFRA_STATIC_ASSERT(!::infra::check_overflow_add<__VA_ARGS__>::overflow)

#define INFRA_STATIC_ASSERT_NO_OVERFLOW_MUL(...) \
    INFRA_STATIC_ASSERT(!::infra::check_overflow_mul<__VA_ARGS__>::overflow)

}  // namespace infra


#endif  // !defined(_INFRA_BASIC_BASIC_MATH_H_INCLUDED_)
