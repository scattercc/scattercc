#include <infra.h>
#include <cstdio>


namespace scattercc
{
    class scatterccd_instance
    {
    public:
        int main(int argc, char* argv[], char* env[])
        {
            infra::maybe_unused(argc, argv, env);

            static constexpr const auto s = infra::make_string('d');
            printf("%s\n", s.value);
            printf("%d\n", (int)s.length());
            INFRA_STATIC_ASSERT(s[0] == 'd');
            INFRA_STATIC_ASSERT(s[1] == '\0');
            INFRA_STATIC_ASSERT(s == "d");

            INFRA_STATIC_ASSERT(infra::make_string('d') == "d");


            static constexpr const auto str = infra::make_string("abc");
            INFRA_STATIC_ASSERT(str == "abc");
            INFRA_STATIC_ASSERT(str.length() == 3);
            INFRA_STATIC_ASSERT(str >= "abc");
            INFRA_STATIC_ASSERT(str <= "abc");
            INFRA_STATIC_ASSERT(str > "ab");
            INFRA_STATIC_ASSERT(str < "abcd");
            INFRA_STATIC_ASSERT(str.repeat<3>() == "abcabcabc");
            INFRA_STATIC_ASSERT(str.repeat<1>() == "abc");
            INFRA_STATIC_ASSERT(str.repeat<0>() == "");
            INFRA_STATIC_ASSERT(str + "d" == "abcd");
            INFRA_STATIC_ASSERT(str + "" == str);
            INFRA_STATIC_ASSERT(str + "d" == str + 'd');
            INFRA_STATIC_ASSERT(str + "de" == str + 'd' + "e");
            INFRA_STATIC_ASSERT(str + infra::make_string('d') == "abcd");
            INFRA_STATIC_ASSERT(str + infra::make_string("d") == "abcd");

            printf("Hello from scatterccd\n");
            return 0;
        }
    };  // class scatterccd_instance

}  // namespace scattercc



int main(int argc, char* argv[], char* env[])
{
    static_assert(__THIS_IS_SCATTERCCD__, "__THIS_IS_SCATTERCCD__ should be true-like");

    scattercc::scatterccd_instance instance;
    return instance.main(argc, argv, env);
}
