#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

Lexer_T *InitializeLexer(char *src)
{
    Lexer_T *NewLexer = calloc(1, sizeof(Lexer_T));

    NewLexer->src = src;
    NewLexer->i   = 0;
    NewLexer->c   = NewLexer->src[NewLexer->i];

    return NewLexer;
}

char LexerPeekOffset(Lexer_T *lexer, int _offset)
{
    return lexer->src[lexer->i + _offset];
}

char *LexerGetVal(Lexer_T *lexer, char _delim)
{
    char *value = calloc(1, sizeof(char));

    while (LexerPeekOffset(lexer, 0) != _delim)
    {
        value = realloc(value, (strlen(value) + 2));

        if (LexerPeekOffset(lexer, 0) == '\\' && LexerPeekOffset(lexer, 1) == 'n')
        {
            strcat(value, (char[]) {'\n', 0});

            for (int i = 0; i < 2; i++)
            {
                LexerAdvanceCharacter(lexer);
            }

            continue;
        }

        strcat(value, (char[]) {LexerPeekOffset(lexer, 0), 0});

        LexerAdvanceCharacter(lexer);
    }

    LexerAdvanceCharacter(lexer);

    return value;
}

void LexerAdvanceCharacter(Lexer_T *lexer)
{
    if (strlen(lexer->src) >= (lexer->i + 1))
    {
        lexer->i++;

        lexer->c = lexer->src[lexer->i];
    }
}

void LexerDevance(Lexer_T *lexer)
{
    if ((lexer->i - 1) >= 0)
    {
        lexer->i = lexer->i - 1;

        lexer->c = lexer->src[lexer->i];
    }
}

void LexerAdvanceOffset(Lexer_T *lexer, int _offset)
{
    for (int i = 0; i < _offset; i++)
    {
        LexerAdvanceCharacter(lexer);
    }
}

char *_ReverseString(char *str)
{
    char *ReversedString = calloc(256, sizeof(char));

    int leng = strlen(str);

    int j = 0;
    int i;

    for (i = (leng - 1); i >= 0; i--)
    {
        ReversedString[j] = str[i];

        j++;
    }

    return ReversedString;
}

char *LexerGetFunctionSrc(Lexer_T *lexer)
{
    char *_src = calloc(1, sizeof(char));

    int _devance = 2;

    /* On minus token */
    LexerDevance(lexer);

    while (isalpha(LexerPeekOffset(lexer, 0)) != 0)
    {
        _src = realloc(_src, (strlen(_src) + 2));

        strcat(_src, (char[]) {LexerPeekOffset(lexer, 0), 0});

        LexerDevance(lexer);

        _devance++;
    }

    LexerAdvanceOffset(lexer, _devance);

    return _ReverseString(_src);
}

char *LexerGetFunctionDest(Lexer_T *lexer)
{
    char *_dest = calloc(1, sizeof(char));

    /* Skip minus token */

    if (LexerPeekOffset(lexer, 0) != '>') {InvalidSyntax(lexer->c, 1);}

    /* Now skip '>' token */

    LexerAdvanceCharacter(lexer);

    while (isalpha(LexerPeekOffset(lexer, 0)) != 0)
    {
        _dest = realloc(_dest, (strlen(_dest) + 2));

        strcat(_dest, (char[]) {LexerPeekOffset(lexer, 0), 0});

        LexerAdvanceCharacter(lexer);
    }

    return _dest;
}

void LexerSkipUntil(Lexer_T *lexer, char ch)
{
    while (LexerPeekOffset(lexer, 0) != ch)
    {
        LexerAdvanceCharacter(lexer);
    }

    LexerAdvanceOffset(lexer, 2);
}

char *LexerGetFunctionArg(Lexer_T *lexer)
{
    /* Skip alpha until over LPAREN */
    LexerSkipUntil(lexer, '(');

    char *val = LexerGetVal(lexer, '"');

    return val;
}

char *LexerAdvanceWithId(Lexer_T *lexer)
{
    char *id = calloc(1, sizeof(id));

    while (isalpha(LexerPeekOffset(lexer, 0)) != 0)
    {
        id = realloc(id, (strlen(id) + 2));

        strcat(id, (char[]) {LexerPeekOffset(lexer, 0), 0});

        LexerAdvanceCharacter(lexer);
    }

    return id;
}

char *LexerAdvanceWithDigit(Lexer_T *lexer)
{
    char *digit = calloc(1, sizeof(char));

    while (isdigit(LexerPeekOffset(lexer, 0)) != 0)
    {
        digit = realloc(digit, (strlen(digit) + 2));

        strcat(digit, (char[]) {LexerPeekOffset(lexer, 0)});

        LexerAdvanceCharacter(lexer);
    }

    return digit;
}

void LexerSkipWhitespace(Lexer_T *lexer)
{
    while (isspace(LexerPeekOffset(lexer, 1)) == 1)
    {
        LexerAdvanceCharacter(lexer);
    }
}

void LexerGenerateAST(Lexer_T *lexer)
{
    AST_Node *Node = ASTGetNextNode(lexer);

    while (Node->Type != TOKEN_EOF)
    {
        if (Node->Type == TOKEN_ID)
        {
            if (strcmp(Node->value, "construct") == 0) {/* Function definition */}
        }

        if (Node->Type == TOKEN_BFUNC)
        {
            //printf("V: '%s', D: '%s' A: '%s'\n", Node->value, Node->_dest, Node->_arg);

            if (strcmp(Node->value, "io") == 0)
            {
                if (strcmp(Node->_dest, "print") == 0)
                {
                    printf("%s", Node->_arg);
                }
            }
        }

        /* Node -> New Node */
        Node = ASTGetNextNode(lexer);
    }
}
