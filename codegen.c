#include "codegen.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BYTEBUFFER *NewBBuffer(void)
{
    BYTEBUFFER *__ptr = calloc(1, sizeof(BYTEBUFFER));

    for (int i = 0; i < 256; i++)
    {
        __ptr->buffer[i] = 0x00;
    }

    return __ptr;
}

FILE *openf(const char *_fname, const char *_fmode)
{
    FILE *__ptr = fopen(_fname, _fmode);

    if (!__ptr) {printf("Error opening file: (%s) in mode: (%s)\n", _fname, _fmode); exit(1);}

    return __ptr;
}

void AssembleCode(char *_fname)
{
    FILE *__ptr = openf(_fname, "rb");

    char *_fdata = calloc(1, sizeof(char));
    char *_fline = calloc(1, sizeof(char));

    size_t Length;
    ssize_t Read;

    while ((Read = getline(&_fline, &Length, __ptr)) != -1)
    {
        size_t _fline_len = strlen(_fline);
        size_t _fdata_len = strlen(_fdata);

        _fdata = realloc(_fdata, (_fdata_len + _fline_len + 1));

        strcat(_fdata, _fline);
    }

    ASMLexer_T *lexer = InitializeASMLexer(_fdata);
}
