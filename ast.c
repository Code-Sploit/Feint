#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int Compare(char *_e1, char *_e2)
{
    if (strcmp(_e1, _e2) == 0) {return 1;}
    if (strcmp(_e1, _e2) != 0) {return 0;}

    return 0;
}

void WriteASMTo(char *_out, char *_data)
{
    FILE *fp = fopen(_out, "w+");

    if (!fp)
    {
        printf("Error could not open '%s' for output asm code!\n");

        exit(1);
    }

    fputs(_data, fp);
    fclose(fp);

    char *_nasm_cmd = calloc(1, sizeof(char));
    char *_link_cmd = calloc(1, sizeof(char));

    _nasm_cmd = realloc(_nasm_cmd, (strlen(_nasm_cmd) + strlen("nasm -f elf64 -o output.out output.s") + 1));
    _link_cmd = realloc(_link_cmd, (strlen(_link_cmd) + strlen("ld output.out -o output") + 1));

    strcat(_nasm_cmd, "nasm -f elf64 -o output.out output.s");
    strcat(_link_cmd, "ld output.out -o output");

    int _nasm = system(_nasm_cmd);
    int _link = system(_link_cmd);

    if (_nasm == 1) {printf("\n\nError while running NASM x86 Assemble of Machine Code!\n");}
    if (_link == 1) {printf("\n\nError while running LD of Assembled output!\n");}
}

void ASTGenerateMachineCode(Scope_T *_Scope, int _debug)
{
    printf("\n\n");

    char *template = calloc(1, sizeof(char));

    template = realloc(template, (strlen(template) + strlen("global _start\n\nsection .text\n\n_start:\nmov rax, 60\nmov rdi, 0\nsyscall") + 1));

    strcat(template, "global _start\n\nsection .text\n\n_start:\nmov rax, 60\nmov rdi, 0\nsyscall");

    int _LastToken = 0;

    for (int i = 0; i < _Scope->_T_count; i++)
    {
        char *val   = _Scope->Tokens[i]->_value;
        char *oper  = _Scope->Tokens[i]->_oper;
        char *name  = _Scope->Tokens[i]->_name;
        char *type  = _Scope->Tokens[i]->_type;
        char *otype = _Scope->Tokens[i]->_otype;

        if (_debug) {printf("V: '%s' O: '%s' N: '%s' T: '%s' O: '%s'\n", val, oper, name, type, otype);}

        if (Compare(otype, "STATEMENT"))
        {
            _LastToken = 0;

            if (Compare(name, "return"))
            {
                /* Return statement */
            }
        }

        if (Compare(otype, "EXPR"))
        {
            if (_LastToken != 1)
            {
                template = realloc(template, strlen(template) + strlen("\nsection .rodata\n") + 1);

                strcat(template, "\nsection .rodata\n");
            }

            _LastToken = 1;

            if (Compare(type, "int"))
            {
                /* Integer definition */
                template = realloc(template, (strlen(template) + strlen(name) + strlen(": db \"") + strlen(val) + strlen("\"") + 1));

                strcat(template, name);
                strcat(template, ": db \"");
                strcat(template, val);
                strcat(template, "\"");
                strcat(template, "\n");

                if (_debug) {printf("%s\n", template);}
            }

            if (Compare(type, "str"))
            {
                /* String definition */
                template = realloc(template, (strlen(template) + strlen(name) + strlen(": db \"") + strlen(val) + strlen("\"") + 1));

                strcat(template, name);
                strcat(template, ": db \"");
                strcat(template, val);
                strcat(template, "\"");
                strcat(template, "\n");

                if (_debug) {printf("%s\n", template);}
            }
        }

        if (!Compare(otype, "EXPR") && !Compare(otype, "STATEMENT"))
        {
            printf("Critical error: [Invalid expression in ASM Generation!]\n");

            exit(1);
        }
    }

    WriteASMTo("output.s", template);
}
