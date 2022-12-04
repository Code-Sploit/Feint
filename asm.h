#include "ast.h"

#include <stdio.h>

typedef struct REGISTER_STRUCT {
    int _inuse;
    int _holding;
} REGISTER;

void GenerateASMCode(AST_T *tree, char *_ofile);

FILE *OpenFile(const char *_fname, const char *_fmode);
