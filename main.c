#include "feint.h"

#include <stdio.h>

void CompilationSuccessfull(char *File)
{
    printf("\nCompilation of: [%s] succeeded!\n", File);
}

void CompilationUnSuccessfull(char *File)
{
    printf("\nCompilation of: [%s] failed!\n", File);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Error: Invalid argument count!\n");
        printf("Syntax: feintc <input file> [compiler args]\n");

        return -1;
    }

    int ret = FeintCompile(argv[1], argv[2], 0);

    if (ret == 0) {CompilationSuccessfull(argv[1]); return 0;}
    if (ret == -1) {CompilationUnSuccessfull(argv[1]); return 1;}

    return 0;
}
