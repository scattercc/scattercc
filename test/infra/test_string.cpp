#include <catch.hpp>
#include <infra.h>

#if SCC_COMPILER_MSVC
// C4146: unary minus operator applied to unsigned type, result still unsigned
#pragma warning (disable: 4146)
#endif

TEST_CASE("Test infra::string", "[infra]")
{
    static constexpr const auto str = infra::make_string("abc") + infra::make_string("def");

    INFRA_STATIC_ASSERT(str + "d" == "abcdefd");
    INFRA_STATIC_ASSERT(str + "" == str);
    INFRA_STATIC_ASSERT(str + "d" == str + 'd');
    INFRA_STATIC_ASSERT(str + "de" == str + 'd' + "e");
    INFRA_STATIC_ASSERT(str + infra::make_string('d') == "abcdefd");
    INFRA_STATIC_ASSERT(str + infra::make_string("d") == "abcdefd");

    INFRA_STATIC_ASSERT(str[0] == 'a');
    INFRA_STATIC_ASSERT(str[1] == 'b');
    INFRA_STATIC_ASSERT(str[2] == 'c');
    INFRA_STATIC_ASSERT(str[3] == 'd');
    INFRA_STATIC_ASSERT(str[4] == 'e');
    INFRA_STATIC_ASSERT(str[5] == 'f');
    INFRA_STATIC_ASSERT(str == "abcdef");
    INFRA_STATIC_ASSERT(str.length() == 6);
    INFRA_STATIC_ASSERT(str > "abc");
    INFRA_STATIC_ASSERT(str >= "abcdef");
    INFRA_STATIC_ASSERT(str <= "abcdef");
    INFRA_STATIC_ASSERT(str == "abcdef");
    INFRA_STATIC_ASSERT(str != "abcdefg");
    INFRA_STATIC_ASSERT(str < "abcdefg");

    INFRA_STATIC_ASSERT(str.find_first_of("xyad") == 0);
    INFRA_STATIC_ASSERT(str.find_first_of("xyde") == 3);
    INFRA_STATIC_ASSERT(str.find_first_of("xy") == infra::size_type(-1));
    INFRA_STATIC_ASSERT(str.find_last_of("xyad") == 3);
    INFRA_STATIC_ASSERT(str.find_last_of("xya!") == 0);
    INFRA_STATIC_ASSERT(str.find_last_of("xy") == infra::size_type(-1));
    INFRA_STATIC_ASSERT(str.find_last_of("\0") == infra::size_type(-1));
    INFRA_STATIC_ASSERT(str.contains_no_null_char());
    INFRA_STATIC_ASSERT(!infra::make_string("abcx\0yz").contains_no_null_char());

    INFRA_STATIC_ASSERT(str.repeat<3>() == "abcdefabcdefabcdef");
    INFRA_STATIC_ASSERT(str.repeat<1>() == "abcdef");
    INFRA_STATIC_ASSERT(str.repeat<0>() == "");

    INFRA_STATIC_ASSERT(str.sub_string<1>() == "bcdef");
    INFRA_STATIC_ASSERT(str.sub_string<3>() == "def");
    INFRA_STATIC_ASSERT(str.sub_string<0>() == "abcdef");
    INFRA_STATIC_ASSERT(str.sub_string<6>() == "");
    INFRA_STATIC_ASSERT(str.sub_string<0, 0>() == "");
    INFRA_STATIC_ASSERT(str.sub_string<1, 0>() == "");
    INFRA_STATIC_ASSERT(str.sub_string<6, 0>() == "");
    INFRA_STATIC_ASSERT(str.sub_string<0, 1>() == "a");
    INFRA_STATIC_ASSERT(str.sub_string<0, 6>() == "abcdef");
    INFRA_STATIC_ASSERT(str.sub_string<1, 4>() == "bcde");
    INFRA_STATIC_ASSERT(str.sub_string<5, 1>() == "f");

    INFRA_STATIC_ASSERT(infra::make_string("abc") == "abc");
    INFRA_STATIC_ASSERT(infra::make_string("abc") + "d" == "abcd");
    INFRA_STATIC_ASSERT(infra::make_string("abc") + "de" == "abcde");
    INFRA_STATIC_ASSERT(infra::make_string("abc") + 'd' == "abcd");
    INFRA_STATIC_ASSERT(infra::make_string("abc") + '\0' == "abc\0");
    INFRA_STATIC_ASSERT(infra::make_string('a') == "a");
    INFRA_STATIC_ASSERT(infra::make_string('\0') == "\0");
    INFRA_STATIC_ASSERT(infra::make_string('\0').length() == 1);

    INFRA_STATIC_ASSERT(infra::make_string("abc").pad_left<3>('_') == "abc");
    INFRA_STATIC_ASSERT(infra::make_string("abc").pad_left<5>('_') == "__abc");
    INFRA_STATIC_ASSERT(infra::make_string("abc").pad_right<3>('_') == "abc");
    INFRA_STATIC_ASSERT(infra::make_string("abc").pad_right<5>('_') == "abc__");


    //INFRA_STATIC_ASSERT(integer_to_string<int, 0>() == "0");
    //INFRA_STATIC_ASSERT(integer_to_string<int, 1>() == "1");
    //INFRA_STATIC_ASSERT(integer_to_string<int, -1>() == "-1");
    //INFRA_STATIC_ASSERT(integer_to_string<std::int32_t, (std::int32_t)0xffffffff, 16>() == "-1");
    //INFRA_STATIC_ASSERT(integer_to_string<std::int32_t, 0x7fffffff, 16>() == "7fffffff");
    //INFRA_STATIC_ASSERT(integer_to_string<std::int32_t, (std::int32_t)0x80000000, 16>() == "-80000000");

    //INFRA_STATIC_ASSERT(integer_to_string<std::uint32_t, 0x0, 16>() == "0");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint32_t, 0x1, 16>() == "1");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint32_t, (std::uint32_t)-1, 16>() == "ffffffff");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint32_t, (std::uint32_t)-2, 16>() == "fffffffe");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint32_t, 0x80000000, 16>() == "80000000");

    //INFRA_STATIC_ASSERT(integer_to_string<std::uint64_t, 0, 16>() == "0");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint64_t, 0x8000000000000000, 16>() == "8000000000000000");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint64_t, -0x8000000000000000, 16>() == "8000000000000000");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint64_t, 0x7fffffffffffffff, 16>() == "7fffffffffffffff");
    //INFRA_STATIC_ASSERT(integer_to_string<std::uint64_t, (std::uint64_t)-0x7fffffffffffffff, 16>() == "8000000000000001");
}
