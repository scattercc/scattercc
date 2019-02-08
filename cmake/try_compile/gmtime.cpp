#include <ctime>
#include <type_traits>
#include <utility>

int main()
{
    //struct tm* gmtime(const time_t* timer);
    static_assert(
        std::is_same<
            decltype(gmtime(std::declval<const time_t*>())),
            tm*
            >::value,
        "fail: gmtime");
    return 0;
}
