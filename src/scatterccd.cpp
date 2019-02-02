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
