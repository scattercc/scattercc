#include <infra.h>
#include <cstdio>

int main(int argc, char* argv[], char* env[])
{
    static_assert(__THIS_IS_SCATTERCCD__, "__THIS_IS_SCATTERCCD__ should be true-like");

    infra::maybe_unused(argc, argv, env);
    infra::maybe_unused<int>();

    printf("Hello from scatterccd\n");
    return 0;
}
