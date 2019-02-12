#if !defined(_INFRA_BASIC_ASSERTION_H_INCLUDED_)
#define _INFRA_BASIC_ASSERTION_H_INCLUDED_

#include <predef/ide.h>
#include <predef/types.h>
#include "./string.h"
#include "./logging.h"
#include "./printf.h"

#include <exception>
#include <cinttypes>
#include <cstring>
#include <string>

namespace infra
{
    namespace details
    {
#define _INFRA_DECLARE_COMPARER(_OpName_, _Op_) \
        template<typename T1, typename T2> \
        struct general_comparer_##_OpName_ \
        { \
            static constexpr bool compare( \
                const typename std::decay<T1>::type& left, \
                const typename std::decay<T2>::type& right \
                ) noexcept \
            { \
                return left _Op_ right; \
            } \
        }; \
        \
        template<typename T1, typename T2> \
        struct string_comparer_##_OpName_ \
        { \
            template<size_t _Size> \
            static const char* cstr(const string<char, _Size>& s) noexcept { return s.value; } \
            template<size_t _Size> \
            static const char* cstr(const char(&s)[_Size]) noexcept { return s; } \
            static const char* cstr(const char* s) noexcept { return s; } \
            static const char* cstr(const std::string& s) noexcept { return s.c_str(); } \
            static bool compare( \
                const typename std::decay<T1>::type& left, \
                const typename std::decay<T2>::type& right \
                ) noexcept \
            { \
                return strcmp(cstr(left), cstr(right)) _Op_ 0; \
            } \
        }
        _INFRA_DECLARE_COMPARER(LT, <);
        _INFRA_DECLARE_COMPARER(LE, <=);
        _INFRA_DECLARE_COMPARER(GT, >);
        _INFRA_DECLARE_COMPARER(GE, >=);
        _INFRA_DECLARE_COMPARER(EQ, ==);
        _INFRA_DECLARE_COMPARER(NE, !=);
#undef _INFRA_DECLARE_COMPARER


        struct printf_transformer
        {
            template<typename T>
            static constexpr
                T* trans(T* const v)
            {
                return v;
            }

            template<typename T>
            static constexpr
                typename std::enable_if<
                    std::is_fundamental<typename std::decay<T>::type>::value &&
                    !std::is_same<typename std::decay<T>::type, bool>::value,
                    const typename std::decay<T>::type&>::type
                trans(const T& v)
            {
                return v;
            }

            template<typename T>
            static constexpr
                typename std::enable_if<std::is_same<typename std::decay<T>::type, bool>::value, const char*>::type
                trans(const T& v)
            {
                return v ? "true" : "false";
            }

            template<typename TStdString>
            static constexpr
                typename std::enable_if<std::is_same<std::string, typename std::decay<TStdString>::type>::value, const char*>::type
                trans(const TStdString& v)
            {
                return v.c_str();
            }

            template<size_t _Size>
            static constexpr const char* trans(const string<char, _Size>& v)
            {
                return v.value;
            }
        };

#ifdef TEST_INFRA_ASSERTION   // See test/infra/test_assertion.cpp for details
        extern "C" {
            extern size_t __assertion_error_count;  // declared in test/infra/test_assertion.cpp
        };
        inline void force_terminate() noexcept
        {
            ++__assertion_error_count;
        }
#else
        [[noreturn]]
        inline void force_terminate()
        {
            std::terminate();
        }
#endif

    }  // namespace details

}  // namespace infra



#define _INFRA_ASSERT_1(_Name_, _What_, _StrWhat_, _UserMsg_, ...) \
    do { \
        auto _value_ = (_What_); \
        if (!(_value_)) \
        { \
            ::infra::simple_date_time_t _now_ { }; \
            (void)now_date_time(&_now_); \
            static constexpr const auto _message_ = ::infra::make_string("") + \
                "[%04u-%02u-%02u %02u:%02u:%02u %s] [PANIC] " + \
                "[" + INFRA_CURRENT_FILE + ":" + INFRA_CURRENT_LINE + "] " + \
                _Name_ + "(" + _StrWhat_ + ") failed. " + \
                _UserMsg_ + "\n"; \
            /* For MSVC, we don't need to "EXPAND" __VA_ARGS__ here, */ \
            /* even if with its strange behavior with __VA_ARGS__. Don't now why. */ \
            /* See: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly */ \
            fprintf(::infra::details::__logging_file, _message_.value, \
                _now_.year, _now_.month, _now_.day_of_month, \
                _now_.hour, _now_.minute, _now_.second, \
                ::infra::details::__infra_time_zone, \
                ##__VA_ARGS__); \
            ::infra::details::force_terminate(); \
        } \
    } while(false)


#if INFRA_IDE_MODE

#define ASSERT(_What_, ...)             printf("" __VA_ARGS__)
#define ASSERT_IS_NULL(_What_, ...)     printf("" __VA_ARGS__)
#define ASSERT_NOT_NULL(_What_, ...)    printf("" __VA_ARGS__)
#define ASSERT_IS_TRUE(_What_, ...)     printf("" __VA_ARGS__)
#define ASSERT_IS_FALSE(_What_, ...)    printf("" __VA_ARGS__)
#define ASSERT_BUG(...)                 printf("" __VA_ARGS__)

#else  // INFRA_IDE_MODE

/* Use JUST to work around MSVC's strange behavior with __VA_ARGS__. */ \
/* See: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly */ \
#define ASSERT(_What_, ...)             JUST(_INFRA_ASSERT_1("ASSERT", (_What_), #_What_, "" __VA_ARGS__))
#define ASSERT_IS_NULL(_What_, ...)     JUST(_INFRA_ASSERT_1("ASSERT_IS_NULL", ((_What_) == nullptr), #_What_, "" __VA_ARGS__))
#define ASSERT_NOT_NULL(_What_, ...)    JUST(_INFRA_ASSERT_1("ASSERT_NOT_NULL", ((_What_) != nullptr), #_What_, "" __VA_ARGS__))
#define ASSERT_IS_TRUE(_What_, ...)     JUST(_INFRA_ASSERT_1("ASSERT_TRUE", ((_What_) == true), #_What_, "" __VA_ARGS__))
#define ASSERT_IS_FALSE(_What_, ...)    JUST(_INFRA_ASSERT_1("ASSERT_FALSE", ((_What_) == false), #_What_, "" __VA_ARGS__))
#define ASSERT_BUG(...)                 JUST(_INFRA_ASSERT_1("ASSERT_BUG", false, "", "Should not reach here! " __VA_ARGS__))

#endif  // INFRA_IDE_MODE


#define _INFRA_ASSERT_COMPARE_2(_Name_, _Comp_, _Left_, _Right_, _StrLeft_, _StrRight_, _UserMsg_, ...) \
    do { \
        auto _left_value_ = (_Left_); \
        auto _right_value_ = (_Right_); \
        if (!(::infra::details::_Comp_<decltype(_Left_), decltype(_Right_)> \
            ::compare(_left_value_, _right_value_))) \
        { \
            ::infra::simple_date_time_t _now_ { }; \
            (void)now_date_time(&_now_); \
            static constexpr const auto _message_ = ::infra::make_string("") + \
                "[%04u-%02u-%02u %02u:%02u:%02u %s] [PANIC] " + \
                "[" + INFRA_CURRENT_FILE + ":" + INFRA_CURRENT_LINE + "] " + \
                _Name_ + "(" + _StrLeft_ + ", " + _StrRight_ + ") failed. " + \
                "LHS: " + _StrLeft_ + " = %" + ::infra::printf_format<decltype(_Left_)>() + ", " + \
                "RHS: " + _StrRight_ + " = %" + ::infra::printf_format<decltype(_Right_)>() + ". " + \
                _UserMsg_ + "\n"; \
            /* For MSVC, we don't need to "EXPAND" __VA_ARGS__ here, */ \
            /* even if with its strange behavior with __VA_ARGS__. Don't now why. */ \
            /* See: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly */ \
            fprintf(::infra::details::__logging_file, _message_.value, \
                _now_.year, _now_.month, _now_.day_of_month, _now_.hour, _now_.minute, _now_.second, \
                ::infra::details::__infra_time_zone, \
                ::infra::details::printf_transformer::trans(_left_value_), \
                ::infra::details::printf_transformer::trans(_right_value_), \
                ##__VA_ARGS__); \
            ::infra::details::force_terminate(); \
        } \
    } while(false)


#if INFRA_IDE_MODE

#define ASSERT_LT(_Left_, _Right_, ...)     printf("" __VA_ARGS__)
#define ASSERT_LE(_Left_, _Right_, ...)     printf("" __VA_ARGS__)
#define ASSERT_GT(_Left_, _Right_, ...)     printf("" __VA_ARGS__)
#define ASSERT_GE(_Left_, _Right_, ...)     printf("" __VA_ARGS__)
#define ASSERT_EQ(_Left_, _Right_, ...)     printf("" __VA_ARGS__)
#define ASSERT_NE(_Left_, _Right_, ...)     printf("" __VA_ARGS__)

#define ASSERT_STR_LT(_Left_, _Right_, ...) printf("" __VA_ARGS__)
#define ASSERT_STR_LE(_Left_, _Right_, ...) printf("" __VA_ARGS__)
#define ASSERT_STR_GT(_Left_, _Right_, ...) printf("" __VA_ARGS__)
#define ASSERT_STR_GE(_Left_, _Right_, ...) printf("" __VA_ARGS__)
#define ASSERT_STR_EQ(_Left_, _Right_, ...) printf("" __VA_ARGS__)
#define ASSERT_STR_NE(_Left_, _Right_, ...) printf("" __VA_ARGS__)

#else  // INFRA_IDE_MODE

/* Use JUST to work around MSVC's strange behavior with __VA_ARGS__. */ \
/* See: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly */ \
#define ASSERT_LT(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_LT", general_comparer_LT, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_LE(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_LE", general_comparer_LE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_GT(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_GT", general_comparer_GT, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_GE(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_GE", general_comparer_GE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_EQ(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_EQ", general_comparer_EQ, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_NE(_Left_, _Right_, ...)     JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_NE", general_comparer_NE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))

#define ASSERT_STR_LT(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_LT", string_comparer_LT, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_STR_LE(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_LE", string_comparer_LE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_STR_GT(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_GT", string_comparer_GT, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_STR_GE(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_GE", string_comparer_GE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_STR_EQ(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_EQ", string_comparer_EQ, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))
#define ASSERT_STR_NE(_Left_, _Right_, ...) JUST(_INFRA_ASSERT_COMPARE_2("ASSERT_STR_NE", string_comparer_NE, (_Left_), (_Right_), #_Left_, #_Right_, "" __VA_ARGS__))

#endif  // INFRA_IDE_MODE


#endif  // !defined(_INFRA_BASIC_ASSERTION_H_INCLUDED_)
