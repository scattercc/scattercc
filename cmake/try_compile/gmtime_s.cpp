#include <ctime>
#include <type_traits>
#include <utility>

int main()
{
    //errno_t gmtime_s(struct tm *result, const time_t* timer);
    static_assert(
        std::is_same<
            decltype(gmtime_s(std::declval<tm*>(), std::declval<const time_t*>())),
            errno_t
            >::value,
        "fail: gmtime_s");
    return 0;
}
