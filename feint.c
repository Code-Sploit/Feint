#include "feint.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int FeintCompile(char *InFile, char *CompilerArguments, int _Bytes)
{
    FILE *InputPointer;

    if (_Bytes == 0) {InputPointer = fopen(InFile, "r");}
    if (_Bytes == 1) {InputPointer = fopen(InFile, "rb");}

    if (!InputPointer)
    {
        printf("Error while opening: [%s]! Quitting...\n", InFile);

        return -1;
    }

    char *FileContents = calloc(1, sizeof(char));
    char *TempStorage;

    size_t Length;
    ssize_t Read;

    while ((Read = getline(&TempStorage, &Length, InputPointer)) != -1)
    {
        FileContents = realloc(FileContents, (strlen(FileContents) + strlen(TempStorage) + 1));

        strcat(FileContents, TempStorage);
    }

    Lexer_T *lexer = InitializeLexer(FileContents);

    /* Get an AST tree of the file contents */

    LexerGenerateAST(lexer);

    return 0;
}
