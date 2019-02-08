#include <ctime>
#include <type_traits>
#include <utility>

int main()
{
    //errno_t localtime_s(struct tm *result, const time_t* timer);
    static_assert(
        std::is_same<
            decltype(localtime_s(std::declval<tm*>(), std::declval<const time_t*>())),
            errno_t
            >::value,
        "fail: localtime_s");
    return 0;
}
