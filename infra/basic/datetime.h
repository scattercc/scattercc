#if !defined(_INFRA_BASIC_DATETIME_H_INCLUDED_)
#define _INFRA_BASIC_DATETIME_H_INCLUDED_

#include <compat/gmtime.h>
#include <compat/localtime.h>

namespace infra
{
    struct simple_date_time_t
    {
        bool is_valid;
        unsigned int year;
        unsigned int month;
        unsigned int day_of_month;
        unsigned int hour;
        unsigned int minute;
        unsigned int second;
    };

    inline bool now_date_time(simple_date_time_t* result)
    {
        do {
            const std::time_t now = std::time(nullptr);

            // If time() could not retrieve the calendar time, it returns a value of -1.
            if (now == std::time_t(-1)) break;

            std::tm local_tm;
            if (!my_localtime(&local_tm, &now)) break;

            result->year = local_tm.tm_year + 1900;
            result->month = local_tm.tm_mon;
            result->day_of_month = local_tm.tm_mday;
            result->hour = local_tm.tm_hour;
            result->minute = local_tm.tm_min;
            result->second = local_tm.tm_sec;
            result->is_valid = true;
            return true;

        } while (false);

        result->year = 0;
        result->month = 0;
        result->day_of_month = 0;
        result->hour = 0;
        result->minute = 0;
        result->second = 0;
        result->is_valid = false;
        return false;
    }

}  // namespace infra

#endif  // !defined(_INFRA_BASIC_DATETIME_H_INCLUDED_)
