#include <catch.hpp>
#include <infra.h>
#include <array>
#include <string>


TEST_CASE("Test infra predef/types.h", "[infra]")
{
    using namespace infra;

    INFRA_STATIC_ASSERT(length_of_array("") == 1);
    INFRA_STATIC_ASSERT(length_of_array("\0") == 2);
    INFRA_STATIC_ASSERT(length_of_array("normal") == 7);
    int dummy_array[] = {1, 2, 3};
    INFRA_STATIC_ASSERT(length_of_array(dummy_array) == 3);
    INFRA_STATIC_ASSERT(length_of_array(std::array<int, 5>()) == 5);

    INFRA_STATIC_ASSERT(length_of_cstr("") == 0);
    INFRA_STATIC_ASSERT(length_of_cstr("\0") == 1);
    INFRA_STATIC_ASSERT(length_of_cstr("normal") == 6);
    INFRA_STATIC_ASSERT(length_of_cstr("ev\0il") == 5);

    INFRA_STATIC_ASSERT(is_decayed_type<int>());
    INFRA_STATIC_ASSERT(is_decayed_type<unsigned long>());
    INFRA_STATIC_ASSERT(is_decayed_type<char>());
    INFRA_STATIC_ASSERT(is_decayed_type<bool>());
    INFRA_STATIC_ASSERT(is_decayed_type<short*>());
    INFRA_STATIC_ASSERT(is_decayed_type<const short*>());
    INFRA_STATIC_ASSERT(is_decayed_type<std::string>());
    INFRA_STATIC_ASSERT(is_decayed_type<std::array<int, 123>>());
    INFRA_STATIC_ASSERT(!is_decayed_type<const int>());
    INFRA_STATIC_ASSERT(!is_decayed_type<volatile unsigned long>());
    INFRA_STATIC_ASSERT(!is_decayed_type<const volatile char>());
    INFRA_STATIC_ASSERT(!is_decayed_type<volatile const bool>());
    INFRA_STATIC_ASSERT(!is_decayed_type<short* const>());
    INFRA_STATIC_ASSERT(!is_decayed_type<const short* const>());
    INFRA_STATIC_ASSERT(!is_decayed_type<std::string&>());
    INFRA_STATIC_ASSERT(!is_decayed_type<std::array<int, 123>&&>());
}
