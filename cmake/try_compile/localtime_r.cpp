#include <ctime>
#include <type_traits>
#include <utility>

int main()
{
    //struct tm* localtime_r(const time_t* timer, struct tm *result);
    static_assert(
        std::is_same<
            decltype(localtime_r(std::declval<const time_t*>(), std::declval<tm*>())),
            tm*
            >::value,
        "fail: localtime_r");
    return 0;
}
