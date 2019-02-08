#include <ctime>
#include <type_traits>
#include <utility>

int main()
{
    //struct tm* localtime(const time_t* timer);
    static_assert(
        std::is_same<
            decltype(localtime(std::declval<const time_t*>())),
            tm*
            >::value,
        "fail: localtime");
    return 0;
}
