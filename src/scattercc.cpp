#include <infra.h>
#include <cstdio>

int main(int argc, char* argv[], char* env[])
{
    infra::maybe_unused(argc, argv, env);
    infra::maybe_unused<int>();

    printf("Hello from scattercc: sizeof(void*) = %d\n", (int)sizeof(void*));
    return 0;
}
