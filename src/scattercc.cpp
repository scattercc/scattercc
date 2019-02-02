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

#if SCC_TARGET_WINDOWS || SCC_TARGET_CYGWIN || SCC_TARGET_MSYS
            const DWORD ret = ::QueueUserAPC([](ULONG_PTR data) {
                printf("[APC] data: %lx\n", static_cast<ULONG>(data));
            }, GetCurrentThread(), 0xdeedbeef);
            printf("QueueUserAPC returns %lu\n", ret);

            if (ret)
            {
                SleepEx(INFINITE, TRUE);
            }
#endif

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
