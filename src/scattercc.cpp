#include <infra.h>
#include <cstdio>


namespace scattercc
{
    class scattercc_instance
    {
    public:
        int main(int argc, char* argv[], char* env[])
        {
            infra::maybe_unused(argc, argv, env);

            printf("Hello from scattercc: sizeof(void*) = %d\n", (int)sizeof(void*));
            return 0;
        }
    };  // class scattercc_instance

}  // namespace scattercc



int main(int argc, char* argv[], char* env[])
{
    static_assert(__THIS_IS_SCATTERCC__, "__THIS_IS_SCATTERCC__ should be true-like");

    scattercc::scattercc_instance instance;
    return instance.main(argc, argv, env);
}
