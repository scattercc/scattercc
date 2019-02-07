#if !defined(_INFRA_BASIC_LOGGING_H_INCLUDED_)
#define _INFRA_BASIC_LOGGING_H_INCLUDED_

#include <predef/ide.h>
#include <predef/static_assert.h>
#include <predef/types.h>
#include "./datetime.h"
#include "./string.h"

#include <cstdio>
#include <cstdint>

namespace infra
{
    namespace details
    {
        extern FILE* __logging_file;  // from infra.cpp
        extern int __logging_verbosity;  // from infra.cpp
        extern char __infra_time_zone[];  // from infra.cpp

        /**
         * NOTE: These variables are used to extract relative paths from absolute paths.
         * For example, for path
         *   "/what/ever/infra/basic/logging.h"
         * we only want the "relative" part:
         *   "infra/basic/logging.hpp"
         *
         * Thus, if the location of current file (logging.h) is changed,
         * the code below should change respectively.
         *
         * Macros:
         *   INFRA_CURRENT_FILE: relative path of current file (type: char[N]).
         *   INFRA_CURRENT_LINE: current line (type: char[N]).
         */
        static constexpr const size_type __source_dir_prefix_length_fname =
            make_string(__FILE__).find_last_of("\\/");
        INFRA_STATIC_ASSERT(__source_dir_prefix_length_fname != size_type(-1));
        INFRA_STATIC_ASSERT(__source_dir_prefix_length_fname > 0);

        static constexpr const size_type __source_dir_prefix_length_infra =
            make_string(__FILE__).find_last_of("\\/", __source_dir_prefix_length_fname - 1);
        INFRA_STATIC_ASSERT(__source_dir_prefix_length_infra != size_type(-1));
        INFRA_STATIC_ASSERT(__source_dir_prefix_length_infra > 0);

        static constexpr const size_type __source_dir_prefix_length_src =
            make_string(__FILE__).find_last_of("\\/", __source_dir_prefix_length_infra - 1);
        INFRA_STATIC_ASSERT(__source_dir_prefix_length_src != size_type(-1));
        INFRA_STATIC_ASSERT_NO_OVERFLOW_ADD(__source_dir_prefix_length_src, 1);

#define INFRA_CURRENT_FILE \
        (::infra::make_string(__FILE__) \
            .sub_string<::infra::details::__source_dir_prefix_length_src + 1>() \
            .value)

#define INFRA_CURRENT_LINE \
        (::infra::integer_to_string<::infra::size_type, ::infra::size_type(__LINE__), 10>().value)

    }  // namespace details

    struct log_level
    {
        enum
        {
            LOG_LEVEL_FATAL = -3,
            LOG_LEVEL_ERROR = -2,
            LOG_LEVEL_WARN = -1,
            LOG_LEVEL_INFO = 0,
            LOG_LEVEL_DBG1 = 1,
            LOG_LEVEL_DBG2 = 2,
            LOG_LEVEL_DBG3 = 3,
            LOG_LEVEL_DBG4 = 4,
            LOG_LEVEL_DBG5 = 5,
        };
    };

}  // namespace infra


#define _INFRA_LOG_INTERNAL(_Level_, _What_, ...) \
    do { \
        if (::infra::details::__logging_verbosity >= ::infra::log_level::LOG_LEVEL_##_Level_) \
        { \
            ::infra::simple_date_time_t _now_ { }; \
            (void)now_date_time(&_now_); \
            /* Here: pad to 7 characters as max log level is 5 characters */ \
            static constexpr const auto _message_ = ::infra::make_string("") + \
                "[%04u-%02u-%02u %02u:%02u:%02u %s] " + \
                (::infra::make_string("[") + #_Level_ + "]").pad_right<7>(' ') + \
                " [" + INFRA_CURRENT_FILE + ":" + INFRA_CURRENT_LINE + "] " + \
                _What_ + "\n"; \
            /* For MSVC, we don't need to "EXPAND" __VA_ARGS__ here, */ \
            /* even if with its strange behavior with __VA_ARGS__. Don't now why. */ \
            /* See: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly */ \
            fprintf(::infra::details::__logging_file, \
                _message_.value, \
                _now_.year, _now_.month, _now_.day_of_month, _now_.hour, _now_.minute, _now_.second, \
                ::infra::details::__infra_time_zone, \
                ##__VA_ARGS__); \
        } \
    } while(false)

#if INFRA_IDE_MODE

#define LOG_FATAL(...)              printf(__VA_ARGS__)
#define LOG_ERROR(...)              printf(__VA_ARGS__)
#define LOG_WARN(...)               printf(__VA_ARGS__)
#define LOG_INFO(...)               printf(__VA_ARGS__)
#define LOG_DBG1(...)               printf(__VA_ARGS__)
#define LOG_DBG2(...)               printf(__VA_ARGS__)
#define LOG_DBG3(...)               printf(__VA_ARGS__)
#define LOG_DBG4(...)               printf(__VA_ARGS__)
#define LOG_DBG5(...)               printf(__VA_ARGS__)

#else  // INFRA_IDE_MODE

#define LOG_FATAL(_What_, ...)      _INFRA_LOG_INTERNAL(FATAL, _What_, ##__VA_ARGS__)
#define LOG_ERROR(_What_, ...)      _INFRA_LOG_INTERNAL(ERROR, _What_, ##__VA_ARGS__)
#define LOG_WARN(_What_, ...)       _INFRA_LOG_INTERNAL(WARN, _What_, ##__VA_ARGS__)
#define LOG_INFO(_What_, ...)       _INFRA_LOG_INTERNAL(INFO, _What_, ##__VA_ARGS__)
#define LOG_DBG1(_What_, ...)       _INFRA_LOG_INTERNAL(DBG1, _What_, ##__VA_ARGS__)
#define LOG_DBG2(_What_, ...)       _INFRA_LOG_INTERNAL(DBG2, _What_, ##__VA_ARGS__)
#define LOG_DBG3(_What_, ...)       _INFRA_LOG_INTERNAL(DBG3, _What_, ##__VA_ARGS__)
#define LOG_DBG4(_What_, ...)       _INFRA_LOG_INTERNAL(DBG4, _What_, ##__VA_ARGS__)
#define LOG_DBG5(_What_, ...)       _INFRA_LOG_INTERNAL(DBG5, _What_, ##__VA_ARGS__)

#endif  // INFRA_IDE_MODE


#endif  // !defined(_INFRA_BASIC_LOGGING_H_INCLUDED_)
