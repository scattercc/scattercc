#include "infra.h"
#include <ctime>
#include <cstdio>
#include <climits>
#include "basic/datetime.h"

namespace infra
{
    namespace details
    {
        FILE* __logging_file = stderr;
        int __logging_verbosity = INT_MAX;
        char __infra_time_zone[64] = "UTC+?";


        struct __infra_initializer_t
        {
            __infra_initializer_t() noexcept
            {
                do {
                    const time_t now = time(nullptr);

                    // If time() could not retrieve the calendar time, it returns a value of -1.
                    if (now == time_t(-1)) break;

                    tm local_time;
                    if (!my_localtime(&local_time, &now)) break;
                    const time_t localt = mktime(&local_time);

                    tm gm_time;
                    if (!my_gmtime(&gm_time, &now)) break;
                    const time_t gmt = mktime(&gm_time);

                    typedef typename std::make_signed<time_t>::type signed_time_t;
                    signed_time_t offset_sec = signed_time_t(localt) - signed_time_t(gmt);

                    const bool is_negative = (offset_sec < 0);
                    if (is_negative)
                    {
                        offset_sec = -offset_sec;
                        if (offset_sec < 0)
                        {
                            break;
                        }
                    }

                    const signed_time_t second = offset_sec % 60;
                    const signed_time_t minute = (offset_sec / 60) % 60;
                    const signed_time_t hour = offset_sec / 3600;
                    if (second == 0)
                    {
                        snprintf(__infra_time_zone,
                            sizeof(__infra_time_zone),
                            (make_string("UTC") +
                                "%c" +  // '+' or '-'
                                "%02" + printf_format<signed_time_t>() +
                                "%02" + printf_format<signed_time_t>()).value,
                            (is_negative ? '-' : '+'),
                            hour,
                            minute);
                    }
                    else  // second > 0
                    {
                        snprintf(__infra_time_zone,
                            sizeof(__infra_time_zone),
                            (make_string("UTC") +
                                "%c" +  // '+' or '-'
                                "%02" + printf_format<signed_time_t>() +
                                "%02" + printf_format<signed_time_t>()).value,
                            (is_negative ? '-' : '+'),
                            hour,
                            minute,
                            second);
                    }
                } while (false);
            }
        } __infra_initializer;

    }  // namespace details

}  // namespace infra