#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int MatchExpr(char *_e1, char *_e2)
{
    if (strcmp(_e1, _e2) == 0) { return 1;}
    if (strcmp(_e1, _e2) != 0) { return 0;}

    return 0;
}

void InvalidSyntax(char syn, int _ret)
{
    printf("Critical error: ['%c'] -> undefined syntax.\n", syn);

    exit(_ret);
}

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

char *LexerGetValue(Lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));

    while (LexerPeekOffset(lexer, 0) != '"')
    {
        value = realloc(value, (strlen(value) + 2));

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

void LexerSkipUntil(Lexer_T *lexer, char ch)
{
    while (LexerPeekOffset(lexer, 0) != ch)
    {
        LexerAdvanceCharacter(lexer);
    }

    LexerAdvanceOffset(lexer, 2);
}

char *LexerAdvanceWithId(Lexer_T *lexer)
{
    char *id = calloc(1, sizeof(id));

    while (isalpha(LexerPeekOffset(lexer, 0)) != 0 || LexerPeekOffset(lexer, 0) == '_')
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

        strcat(digit, (char[]) {lexer->c, 0});

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

Token_Node *LexerSkipToken(Lexer_T *lexer, int _count)
{
    Token_Node *tok = calloc(1, sizeof(Token_Node));

    for (int i = 0; i < _count; i++)
    {
        tok = LexerGetNextToken(lexer);
    }

    return tok;
}

Token_Node *LexerGetNextToken(Lexer_T *lexer)
{
    LexerSkipWhitespace(lexer);

    while (LexerPeekOffset(lexer, 1) != '\0')
    {

        if (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t') {LexerAdvanceCharacter(lexer); continue;}

        if (isalpha(lexer->c) != 0) {return InitializeToken(LexerAdvanceWithId(lexer), TOKEN_ID);}
        if (isdigit(lexer->c) != 0) {return InitializeToken(LexerAdvanceWithDigit(lexer), TOKEN_DIGIT);}

        if (isspace(lexer->c) == 0 && isalpha(lexer->c) == 0 && isdigit(lexer->c) == 0)
        {
            switch (lexer->c)
            {
                case '!': {LexerAdvanceCharacter(lexer); return InitializeToken("!", TOKEN_EXCL);}
                case '@': {LexerAdvanceCharacter(lexer); return InitializeToken("@", TOKEN_AT);}
                case '#': {LexerAdvanceCharacter(lexer); return InitializeToken("#", TOKEN_HASHTAG);}
                case '$': {LexerAdvanceCharacter(lexer); return InitializeToken("$", TOKEN_DOLLAR);}
                case '%': {LexerAdvanceCharacter(lexer); return InitializeToken("%", TOKEN_PERCENT);}
                case '^': {LexerAdvanceCharacter(lexer); return InitializeToken("^", TOKEN_UP);}
                case '&': {LexerAdvanceCharacter(lexer); return InitializeToken("&", TOKEN_ZS);}
                case '*': {LexerAdvanceCharacter(lexer); return InitializeToken("*", TOKEN_STAR);}
                case '(': {LexerAdvanceCharacter(lexer); return InitializeToken("(", TOKEN_LPAREN);}
                case ')': {LexerAdvanceCharacter(lexer); return InitializeToken(")", TOKEN_RPAREN);}
                case '+': {LexerAdvanceCharacter(lexer); return InitializeToken("+", TOKEN_PLUS);}
                case '=': {LexerAdvanceCharacter(lexer); return InitializeToken("=", TOKEN_IS);}
                case '{': {LexerAdvanceCharacter(lexer); return InitializeToken("{", TOKEN_RBRACE);}
                case '}': {LexerAdvanceCharacter(lexer); return InitializeToken("}", TOKEN_LBRACE);}
                case ';': {LexerAdvanceCharacter(lexer); return InitializeToken(";", TOKEN_SEMICOLON);}
                case ',': {LexerAdvanceCharacter(lexer); return InitializeToken(",", TOKEN_COMMA);}
                case '"': {LexerAdvanceCharacter(lexer); return InitializeToken(LexerGetValue(lexer), TOKEN_ID);}

                default: InvalidSyntax(lexer->c, 1);
            }
        }

        LexerAdvanceCharacter(lexer);
    }

    return InitializeToken("\0", TOKEN_EOF);
}
