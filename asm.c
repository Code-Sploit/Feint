#include "asm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REGISTER_COUNT 6

REGISTER *REGISTERS[REGISTER_COUNT];

char REGISTER_ALPHABET[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

FILE *OpenFile(const char *_fname, const char *_fmode)
{
    FILE *__ptr = fopen(_fname, _fmode);

    if (!__ptr)
    {
        printf("Error while opening file: (%s) in mode: (%s). Aborting compilation process!\n", _fname, _fmode);

        exit(1);
    }

    return __ptr;
}

void AllocateRegisters(void)
{
    for (int i = 0; i < sizeof(REGISTER_COUNT); i++)
    {
        REGISTERS[i] = calloc(1, sizeof(REGISTER));
    }
}

char *RegisterIndexToStr(int _index)
{
    char *__ptr = calloc(1, sizeof(char));

    __ptr = realloc(__ptr, (strlen(__ptr) + 2));

    strcat(__ptr, (char[]) {REGISTER_ALPHABET[_index], 0});

    return __ptr;
}

char *AllocateRegister(char *_vname, char *_vvalue)
{
    for (int i = 0; i < REGISTER_COUNT; i++)
    {
        int _isactive = REGISTERS[i]->_inuse;

        if (_isactive == 0)
        {
            REGISTERS[i]->_inuse = 1;

            return RegisterIndexToStr(i);
        }
    }

    printf("\nCritical error ran out of registers! Aborting compilation process!\n");

    return "\0";
}

char *AssignMainFunction(AST_T *tree, char *_odata)
{
    char *fdata    = calloc(1, sizeof(char));
    char *template = calloc(1, sizeof(char));
    char *_fname   = tree->_fname;

    size_t _sdata = strlen(_odata);
    size_t _ndata = strlen(template);

    printf("\nAssigning main function: Name: (%s)\n", _fname);

    /* TEMPLATE: 
        global <fname>

        fname:
        (code)
    */

    template = realloc(template, (_sdata + _ndata + strlen("global ") + (2 * strlen(_fname)) + strlen("\n:\n")) + 1);

    /* Strcat the stuff into our template */

    strcat(template, "global _");
    strcat(template, _fname);
    strcat(template, "\n_");
    strcat(template, _fname);
    strcat(template, ":\n");

    /* Push the template into fdata */

    fdata = realloc(fdata, (strlen(fdata) + strlen(template) + 1));

    strcat(fdata, template);

    return fdata;
}

char *GenerateDeclarationCode(AST_T *node)
{
    char *name  = node->_vname;
    char *value = node->_vvalue;
    
    char *regis = AllocateRegister(name, value);

    char *template = calloc(1, sizeof(char));

    /* TEMPLATE:
        mov r<x>, <value>
    */

    template = realloc(template, (strlen(template) + strlen("mov ex, ") + strlen(value) + strlen("\n")));

    strcat(template, "mov e");
    strcat(template, regis);
    strcat(template, "x, ");
    strcat(template, value);
    strcat(template, "\n");

    return template;
}

char *GenerateReturnCode(AST_T *node)
{
    char *ret = node->_rvalue;

    char *template = calloc(1, sizeof(char));

    /* TEMPLATE:
        mov rax, 60
        mov rdi, <ret value>
        syscall
    */

   template = realloc(template, (strlen(template) + strlen("mov rax, 60\nmov rdi, ") + strlen(ret) + strlen("\nsyscall\n")) + 1);

   strcat(template, "mov rax, 60\nmov rdi, ");
   strcat(template, ret);
   strcat(template, "\nsyscall\n");

   return template;
}

char *AssignMainScope(AST_T *tree, char *final_data)
{
    char *_fdata = calloc(1, sizeof(char));

    /* First of all copy old data to new variable */
    
    _fdata = realloc(_fdata, (strlen(_fdata) + strlen(final_data) + 1));

    strcat(_fdata, final_data);

    for (int i = 0; i < tree->_cc; i++)
    {
        AST_T *node = tree->children[i];

        if (node->type == AST_DECLARATION)
        {
            /* Looks like we got a declaration */
            /* For now everything is int */

            /* TOOD: Add string handling */
            char *_code = GenerateDeclarationCode(node);

            _fdata = realloc(_fdata, (strlen(_fdata) + strlen(_code)) + 1);

            strcat(_fdata, _code);
        }

        if (node->type == AST_STATEMENT)
        {
            /* Statement! */
            /* For now every statement is currently return */

            /* TODO: Add while and for loops */
            char *_code = GenerateReturnCode(node);

            _fdata = realloc(_fdata, (strlen(_fdata) + strlen(_code)) + 1);

            strcat(_fdata, _code);
        }
    }

    return _fdata;
}

char *TreeToAssembly(AST_T *tree)
{
    char *final_data = calloc(1, sizeof(char));

    /* Add the main function to the tree */

    final_data = AssignMainFunction(tree, final_data);
    final_data = AssignMainScope(tree, final_data);

    return final_data;
}

void GenerateASMCode(AST_T *tree, char *_ofile)
{
    /* Allocate log-registers */
    AllocateRegisters();

    FILE *__ptr = OpenFile(_ofile, "w+");

    char *_fdata = TreeToAssembly(tree);

    fputs(_fdata, __ptr);
    fclose(__ptr);

    printf("\nAssembly code finished!\n");
}
