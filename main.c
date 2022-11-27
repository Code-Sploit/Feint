#include "feint.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void CompilationSuccessfull(char *File, char *OFile)
{
    if (strcmp(OFile, "NULL") == 0)
    {
        printf("\nCompilation of: [\033[0;36m%s\033[0m] \033[0;32msucceeded\033[0m!\n\nSaved executable file to: [\033[0;36m./output\033[0m]", File);
    }
    else
    {
        printf("\nCompilation of: [\033[0;36m%s\033[0m] \033[0;32msucceeded\033[0m!\n\nSaved executable file to: [\033[0;36m./%s\033[0m]", File, OFile);
    }
}

void CompilationUnSuccessfull(char *File)
{
    printf("\nCompilation of: [\033[0;36m%s\033[0m] \033[0;31mfailed\033[0m!\n", File);
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("Error: Invalid argument count!\n");
        printf("Syntax: feintc <input file> -o <output file>\n");

        return -1;
    }

    char *_IFile = argv[1];
    char *_OFile = argv[3];
    char *__D    = argv[4];

    int __ret = FeintCompile(_IFile, __D, _OFile, 0);

    if (__ret == 0)
    {
        CompilationSuccessfull(_IFile, _OFile);
    }
    else
    {
        CompilationUnSuccessfull(_IFile);
    }

    return 0;
}
