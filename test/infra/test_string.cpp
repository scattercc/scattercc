#include <catch.hpp>
#include <infra.h>

TEST_CASE("Test infra::string", "[infra]")
{
    static constexpr const auto str = "abc" + infra::make_string("def");

    INFRA_STATIC_ASSERT(str + "d" == "abcdefd");
    INFRA_STATIC_ASSERT(str + "" == str);
    INFRA_STATIC_ASSERT(str + "d" == str + 'd');
    INFRA_STATIC_ASSERT(str + "de" == str + 'd' + "e");
    INFRA_STATIC_ASSERT(str + infra::make_string('d') == "abcdefd");
    INFRA_STATIC_ASSERT(str + infra::make_string("d") == "abcdefd");
    INFRA_STATIC_ASSERT("x" + str == "xabcdef");
    INFRA_STATIC_ASSERT('x' + str == "xabcdef");
    INFRA_STATIC_ASSERT("xy" + str == "xyabcdef");

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
    INFRA_STATIC_ASSERT(str.find_first_of("xy") == infra::size_t(-1));
    INFRA_STATIC_ASSERT(str.find_last_of("xyad") == 3);
    INFRA_STATIC_ASSERT(str.find_last_of("xya!") == 0);
    INFRA_STATIC_ASSERT(str.find_last_of("xy") == infra::size_t(-1));
    INFRA_STATIC_ASSERT(str.find_last_of("\0") == infra::size_t(-1));
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
}
