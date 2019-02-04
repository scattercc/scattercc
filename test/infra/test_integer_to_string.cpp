#include <catch.hpp>
#include <infra.h>

#if SCC_COMPILER_MSVC
// C4146: unary minus operator applied to unsigned type, result still unsigned
#pragma warning (disable: 4146)
#endif

TEST_CASE("Test infra::integer_to_string", "[infra]")
{
    INFRA_STATIC_ASSERT(infra::integer_to_string<int, 0>() == "0");
    INFRA_STATIC_ASSERT(infra::integer_to_string<int, 1>() == "1");
    INFRA_STATIC_ASSERT(infra::integer_to_string<int, -1>() == "-1");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::int32_t, std::int32_t(0xffffffff), 16>() == "-1");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::int32_t, 0x7fffffff, 16>() == "7fffffff");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::int32_t, std::int32_t(0x80000000), 16>() == "-80000000");

    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint32_t, 0x0, 16>() == "0");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint32_t, 0x1, 16>() == "1");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint32_t, std::uint32_t(-1), 16>() == "ffffffff");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint32_t, std::uint32_t(-2), 16>() == "fffffffe");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint32_t, 0x80000000, 16>() == "80000000");

    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint64_t, 0, 16>() == "0");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint64_t, 0x8000000000000000, 16>() == "8000000000000000");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint64_t, -0x8000000000000000, 16>() == "8000000000000000");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint64_t, 0x7fffffffffffffff, 16>() == "7fffffffffffffff");
    INFRA_STATIC_ASSERT(infra::integer_to_string<std::uint64_t, std::uint64_t(-0x7fffffffffffffff), 16>() == "8000000000000001");
}
