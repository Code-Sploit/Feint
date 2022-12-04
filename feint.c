#include "feint.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int FeintCompile(char *InFile, char *CompilerArguments, char *OFile, int _Bytes)
{
    printf("\nStarting compilation process!... Targets: [\033[1;36m%s\033[0m]\n", InFile);

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

    /* We have the file data so let's make an AST_T object */
    AST_T *tree = GenerateASTTree(FileContents);

    for (int i = 0; i < tree->_cc; i++)
    {
        printf("Variable type: (%d) stable: (%d) value: (%s)\n", tree->children[i]->vtype, tree->children[i]->_isstable, tree->children[i]->_vvalue);
    }

    /* Run a syntax check */
    //SyntaxChecker(tree);

    /* Seems like everything went well */
    /* Generate ASM code */
    GenerateASMCode(tree, OFile);

    /* Assemble ASM code */
    AssembleCode(OFile);

    /* Done! */
    return 0;
}
