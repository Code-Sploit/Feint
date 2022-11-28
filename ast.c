#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *IntToStr(int _N)
{
    char *NewStr = calloc(1, sizeof(char));

    sprintf(NewStr, "%d", _N);

    return NewStr;
}

int Compare(char *_e1, char *_e2)
{
    if (strcmp(_e1, _e2) == 0) {return 1;}
    if (strcmp(_e1, _e2) != 0) {return 0;}

    return 0;
}

char *LenExt(char *_e)
{
    char *ret = calloc(1, sizeof(char));

    ret = realloc(ret, (strlen(_e) + strlen("len\n") + 1));

    strcat(ret, _e);
    strcat(ret, "len");

    return ret;
}

char *ScopeExt(char *_e, char *_scope)
{
    char *ret = calloc(1, sizeof(char));

    ret = realloc(ret, (strlen(_e) + strlen(_scope) + strlen("_") + 1));

    strcat(ret, _scope);
    strcat(ret, "_");
    strcat(ret, _e);

    return ret;
}

char *AccessVariable(char *_odata, char *_vname)
{
    if (strcmp(_odata, "") == 0) {return _odata;}

    char *_oname = calloc(1, sizeof(char));
    char *_nval  = calloc(1, sizeof(char));

    int _loop = 0;

    while (_loop <= strlen(_odata))
    {
        while (isalpha(_odata[_loop]) != 0 || _odata[_loop] == '_')
        {
            _oname = realloc(_oname, (strlen(_oname) + 2));

            strcat(_oname, (char[]) {_odata[_loop], 0});

            _loop++;
        }

        if (strcmp(_oname, _vname) == 0)
        {
            /* We found the variable we needed to visit now let's get the actual value */

            _loop++;

            while (_odata[_loop] != '"')
            {
                _loop++;
            }

            _loop++;

            while (_odata[_loop] != '"')
            {
                _nval = realloc(_nval, (strlen(_nval) + 2));

                strcat(_nval, (char[]) {_odata[_loop], 0});

                _loop++;
            }

            break;
        }

        _loop++;

        memset(_oname, 0, strlen(_oname));
    }

    return _nval;
}

char *UpdateROData(char *_odata, char *_vname, char *_nval)
{   
    if (strcmp(_odata, "") == 0) {return _odata;}

    char *_oname = calloc(1, sizeof(char));
    char *_ndata = calloc(1, sizeof(char));

    int _loop = 0;

    while (_loop <= strlen(_odata))
    {
        while (isalpha(_odata[_loop]) != 0 || _odata[_loop] == '_' || _odata[_loop] == '.')
        {
            _oname = realloc(_oname, (strlen(_oname) + 1));

            strcat(_oname, (char[]) {_odata[_loop], 0});

            _loop++;
        }

        if (strcmp(_vname, _oname) == 0)
        {
            /* Change the line */
            _ndata = realloc(_ndata, (strlen(_ndata) + strlen(_vname) + strlen(": db \"") + strlen(_nval) + strlen("\", ENDL\n") + 1));

            strcat(_ndata, _vname);
            strcat(_ndata, ": db \"");
            strcat(_ndata, _nval);
            strcat(_ndata, "\", ENDL");

            /* Skip until next line */
            while (_odata[_loop] != '\n')
            {
                _loop++;
            }
        }
        else if (strcmp(_vname, "section") == 0)
        {
            _ndata = realloc(_ndata, (strlen(_ndata) + strlen("section .rodata\n")) + 1);

            strcat(_ndata, "section .rodata\n");

            /* Skip until next line */
            while (_odata[_loop] != '\n')
            {
                _loop++;
            }
        }
        else
        {
            /* Is the word valid? */
            if (_odata[_loop] == ':')
            {
                /* First add the word that was not our variable but still add it because it was another */
                _ndata = realloc(_ndata, (strlen(_ndata) + strlen(_oname) + 1));

                strcat(_ndata, _oname);

                /* Get all data until '\n' symbol and add to _ndata */
                while (_odata[_loop] != '\n')
                {
                    _ndata = realloc(_ndata, (strlen(_ndata) + 1));

                    strcat(_ndata, (char[]) {_odata[_loop], 0});

                    _loop++;
                }
            }
        }

        _ndata = realloc(_ndata, (strlen(_ndata) + 2));

        strcat(_ndata, (char[]) {_odata[_loop], 0});

        _loop++;

        /* Reset _oname string */
        memset(_oname, 0, strlen(_oname));
    }

    /* Remove weird end line */
    char *wline = strchr(_ndata, '!');

    if (wline != NULL) {*wline = '\0';}

    return _ndata;
}

void WriteASMTo(char *_out, char *_start_function_data, char *_declarations, char **_functions, int _F_Count)
{
    char *_out_s = calloc(1, sizeof(char));

    _out_s = realloc(_out_s, (strlen(_out_s) + strlen(".s")) + 1);

    strcat(_out_s, _out);
    strcat(_out_s, ".s");

    FILE *fp = fopen(_out_s, "w+");

    if (!fp)
    {
        printf("Error could not open '%s' for output asm code!\n", _out_s);

        exit(1);
    }

    char *_StandardDeclarations = "%define ENDL 0x0D, 0x0A\n";

    fputs(_StandardDeclarations, fp);

    for (int i = 0; i < _F_Count; i++)
    {
        fputs(_functions[i], fp);
    }

    fputs("\n", fp);

    fputs(_start_function_data, fp);
    fputs(_declarations, fp);

    fclose(fp);

    char *_nasm_cmd = calloc(1, sizeof(char));
    char *_link_cmd = calloc(1, sizeof(char));

    _nasm_cmd = realloc(_nasm_cmd, (strlen(_nasm_cmd) + strlen("nasm -f elf64 -o ") + (2 * strlen(_out)) + strlen(".out") + strlen(" .s") + 1));
    _link_cmd = realloc(_link_cmd, (strlen(_link_cmd) + strlen("ld ") + (2 * strlen(_out)) + strlen("-o .s") + 1));

    strcat(_nasm_cmd, "nasm -f elf64 -o ");
    strcat(_nasm_cmd, _out);
    strcat(_nasm_cmd, ".out ");
    strcat(_nasm_cmd, _out);
    strcat(_nasm_cmd, ".s");

    strcat(_link_cmd, "ld ");
    strcat(_link_cmd, _out);
    strcat(_link_cmd, ".out");
    strcat(_link_cmd, " -o ");
    strcat(_link_cmd, _out);

    int _nasm = system(_nasm_cmd);
    int _link = system(_link_cmd);

    if (_nasm == 1) {printf("\n\nError while running NASM x86 Assemble of Machine Code!\n");}
    if (_link == 1) {printf("\n\nError while running LD of Assembled output!\n");}
}

void ASTGenerateMachineCode(Scope_T *_Scope, char *OFile, int _debug)
{
    char *_asm_rodata   = calloc(1, sizeof(char));
    char *_asm_start_f  = calloc(1, sizeof(char));

    char *_asm_functions[256];

    int _F_Count = 0;

    /* Allocate array */
    for (int i = 0; i < 256; i++)
    {
        _asm_functions[i] = calloc(1, sizeof(char));
    }

    _asm_start_f = realloc(_asm_start_f, (strlen(_asm_start_f) + strlen("global _start\n\nsection .text\n\n_start:\n") + 1));
    _asm_rodata = realloc(_asm_rodata, (strlen(_asm_rodata) + strlen("\nsection .rodata\n") + 1));

    strcat(_asm_start_f, "global _start\n\nsection .text\n\n_start:\n");
    strcat(_asm_rodata, "\nsection .rodata\n");

    for (int i = 0; i < _Scope->_T_count; i++)
    {
        char *val   = _Scope->Tokens[i]->_value;
        char *oper  = _Scope->Tokens[i]->_oper;
        char *name  = _Scope->Tokens[i]->_name;
        char *type  = _Scope->Tokens[i]->_type;
        char *scope = _Scope->Tokens[i]->_scope;
        char *otype = _Scope->Tokens[i]->_otype;

        if (_debug) {printf("V: '%s' O: '%s' N: '%s' T: '%s' S: '%s' O: '%s'\n", val, oper, name, type, scope, otype);}

        if (Compare(otype, "STATEMENT"))
        {
            if (Compare(name, "return"))
            {
                /* Return statement */
                _asm_start_f = realloc(_asm_start_f, (strlen(_asm_start_f) + (strlen("mov rax, 60\nmov rdi,") + strlen(val) + strlen("\nsyscall\n") + 1)));

                strcat(_asm_start_f, "mov rax, 60\nmov rdi, ");
		        strcat(_asm_start_f, val);
		        strcat(_asm_start_f, "\nsyscall\n");

                if (_debug) {printf("%s\n", _asm_start_f);}

                if (Compare(name, "main")) {break;}
            }
        }

        if (Compare(otype, "EXPR"))
        {
            if (Compare(type, "int"))
            {
                /* Integer definition */
                _asm_rodata = realloc(_asm_rodata, (strlen(_asm_rodata) + strlen(name) + strlen(": db \"") + strlen(ScopeExt(val, scope)) + strlen("\", ENDL") + 1));

                strcat(_asm_rodata, ScopeExt(name, scope));
                strcat(_asm_rodata, ": db \"");
                strcat(_asm_rodata, val);
                strcat(_asm_rodata, "\", ENDL");
                strcat(_asm_rodata, "\n");

                _asm_rodata = realloc(_asm_rodata, (strlen(_asm_rodata) + strlen(LenExt(name)) + strlen(": equ $ - ") + strlen(ScopeExt(name, scope)) + strlen("\n") + 1));

                strcat(_asm_rodata, LenExt(name));
                strcat(_asm_rodata, ": equ $ - ");
                strcat(_asm_rodata, ScopeExt(name, scope));
                strcat(_asm_rodata, "\n");

                if (_debug) {printf("%s\n", _asm_rodata);}
            }

            if (Compare(type, "str"))
            {
                /* String definition */
                _asm_rodata = realloc(_asm_rodata, (strlen(_asm_rodata) + strlen(name) + strlen(": db \"") + strlen(ScopeExt(val, scope)) + strlen("\", ENDL") + 1));

                strcat(_asm_rodata, ScopeExt(name, scope));
                strcat(_asm_rodata, ": db \"");
                strcat(_asm_rodata, val);
                strcat(_asm_rodata, "\", ENDL");
                strcat(_asm_rodata, "\n");

                _asm_rodata = realloc(_asm_rodata, (strlen(_asm_rodata) + strlen(LenExt(name)) + strlen(": equ $ - ") + strlen(ScopeExt(name, scope)) + strlen("\n") + 1));

                strcat(_asm_rodata, LenExt(name));
                strcat(_asm_rodata, ": equ $ - ");
                strcat(_asm_rodata, ScopeExt(name, scope));
                strcat(_asm_rodata, "\n");

                if (_debug) {printf("%s\n", _asm_rodata);}
            }

            if (Compare(name, "printvar"))
            {
                if (Compare(scope, "main"))
                {
                    _asm_start_f = realloc(_asm_start_f, (strlen(_asm_start_f) + strlen("mov rax, 1\nmov rdi, 1\nmov rsi, ") + strlen(val) + strlen("\nmov rdx, ") + strlen(LenExt(val)) + strlen("\nsyscall\n")) + 1);

                    strcat(_asm_start_f, "mov rax, 1\nmov rdi, 1\nmov rsi, ");
                    strcat(_asm_start_f, ScopeExt(val, scope));
                    strcat(_asm_start_f, "\nmov rdx, ");
                    strcat(_asm_start_f, LenExt(val));
                    strcat(_asm_start_f, "\nsyscall\n");

                    if (_debug) {printf("%s\n", _asm_start_f);}
                }
                else
                {
                    _asm_functions[_F_Count] = realloc(_asm_functions[_F_Count], (strlen(_asm_functions[_F_Count]) + strlen(scope) + strlen(":\nmov rax, 1\nmov rdi, 1\nmov rsi, ") + strlen(val) + strlen("\nmov rdx, ") + strlen(LenExt(val)) + strlen("\nsyscall\n")) + 1);

                    strcat(_asm_functions[_F_Count], scope);
                    strcat(_asm_functions[_F_Count], ":\nmov rax, 1\nmov rdi, 1\nmov rsi, ");
                    strcat(_asm_functions[_F_Count], ScopeExt(val, scope));
                    strcat(_asm_functions[_F_Count], "\nmov rdx, ");
                    strcat(_asm_functions[_F_Count], LenExt(val));
                    strcat(_asm_functions[_F_Count], "\nsyscall\n");

                    if (_debug) {printf("%s\n", _asm_functions[_F_Count]);}
                }
            }

            if (Compare(name, "call"))
            {
                if (Compare(scope, "main"))
                {
                    _asm_start_f = realloc(_asm_start_f, (strlen(_asm_start_f) + strlen("call ") + strlen(val) + strlen("\n") + 1));

                    strcat(_asm_start_f, "call ");
                    strcat(_asm_start_f, val);
                    strcat(_asm_start_f, "\n");
                    
                    if (_debug) {printf("%s\n", _asm_start_f);}
                }
                else
                {
                    _asm_functions[_F_Count] = realloc(_asm_functions[_F_Count], (strlen(_asm_functions[_F_Count] + strlen("call ") + strlen(val) + strlen("\n") + 1)));

                    strcat(_asm_functions[_F_Count], "call ");
                    strcat(_asm_functions[_F_Count], val);
                    strcat(_asm_functions[_F_Count], "\n");

                    if (_debug) {printf("%s\n", _asm_functions[_F_Count]);}
                }
            }

            if (Compare(name, "back"))
            {
                if (Compare(scope, "main"))
                {
                    _asm_start_f = realloc(_asm_start_f, (strlen(_asm_start_f) + strlen("ret\n") + 1));

                    strcat(_asm_start_f, "ret\n");

                    if (_debug) {printf("%s\n", _asm_start_f);}
                }
                else
                {
                    _asm_functions[_F_Count] = realloc(_asm_functions[_F_Count], (strlen(_asm_functions[_F_Count]) + strlen("ret\n") + 1));

                    strcat(_asm_functions[_F_Count], "ret\n");

                    if (_debug) {printf("%s\n", _asm_functions[_F_Count]);}
                }
            }
        }

        if (Compare(otype, "ARG"))
        {
            if (Compare(type, "sum"))
            {
                int _A = atoi(AccessVariable(_asm_rodata, ScopeExt(name, scope)));
                int _B = atoi(AccessVariable(_asm_rodata, ScopeExt(val, scope)));
                int _R = _A + _B;

                _asm_rodata = UpdateROData(_asm_rodata, ScopeExt(_Scope->Tokens[i]->_second_argument, scope), IntToStr(_R));

                if (_debug) {printf("%s\n", _asm_rodata);}
            }

            if (Compare(type, "sub"))
            {
                int _A = atoi(AccessVariable(_asm_rodata, ScopeExt(name, scope)));
                int _B = atoi(AccessVariable(_asm_rodata, ScopeExt(val, scope)));
                int _R = _A - _B;

                _asm_rodata = UpdateROData(_asm_rodata, ScopeExt(_Scope->Tokens[i]->_second_argument, scope), IntToStr(_R));

                if (_debug) {printf("%s\n", _asm_rodata);}
            }

            if (Compare(type, "mul"))
            {
                int _A = atoi(AccessVariable(_asm_rodata, ScopeExt(name, scope)));
                int _B = atoi(AccessVariable(_asm_rodata, ScopeExt(val, scope)));
                int _R = _A * _B;

                _asm_rodata = UpdateROData(_asm_rodata, ScopeExt(_Scope->Tokens[i]->_second_argument, scope), IntToStr(_R));

                if (_debug) {printf("%s\n", _asm_rodata);}
            }

            if (Compare(type, "div"))
            {
                int _A = atoi(AccessVariable(_asm_rodata, ScopeExt(name, scope)));
                int _B = atoi(AccessVariable(_asm_rodata, ScopeExt(val, scope)));
                int _R = _A / _B;

                _asm_rodata = UpdateROData(_asm_rodata, ScopeExt(_Scope->Tokens[i]->_second_argument, scope), IntToStr(_R));

                if (_debug) {printf("%s\n", _asm_rodata);}
            }
        }

        _F_Count++;

        if (!Compare(otype, "EXPR") && !Compare(otype, "STATEMENT") && !(Compare(otype, "ARG")))
        {
            printf("Critical error: [Invalid expression in ASM Generation!]\n");

            exit(1);
        }
    }

    WriteASMTo(OFile, _asm_start_f, _asm_rodata, _asm_functions, _F_Count);
}
