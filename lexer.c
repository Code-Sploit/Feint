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

char *LexerGetVal(Lexer_T *lexer, char *_delim)
{
    char *value = calloc(1, sizeof(char));

    while (isspace(LexerPeekOffset(lexer, 1)) == 0 && LexerPeekOffset(lexer, 1) != '"')
    {
        value = realloc(value, (strlen(value) + 2));

        strcat(value, (char[]) {LexerPeekOffset(lexer, 1), 0});

        LexerAdvanceCharacter(lexer);
    }

    return value;
}

void LexerAdvanceCharacter(Lexer_T *lexer)
{
    lexer->i++;

    lexer->c = lexer->src[lexer->i];
}

void LexerAdvanceWithId(Lexer_T *lexer)
{
    /* printf("%c", lexer->c); */

    while (isalpha(LexerPeekOffset(lexer, 1)) != 0)
    {
        LexerAdvanceCharacter(lexer);

        /* printf("%c", lexer->c); */
    }

    /* printf("\n"); */
}

void LexerAdvanceWithDigit(Lexer_T *lexer)
{
    while (isdigit(LexerPeekOffset(lexer, 1)) == 1)
    {
        LexerAdvanceCharacter(lexer);
    }
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
    AST_Node *Nodes[256];

    for (int i = 0; i < 256; i++)
    {
        Nodes[i] = calloc(1, sizeof(AST_Node));
    }

    /* Get all tokens */
    AST_Node *Node;

    while ((Node = ASTGetNextNode(lexer)))
    {
        Nodes[(sizeof(Nodes) / sizeof(Nodes[0])) + 1] = Node;

        if (Node->Type == TOKEN_VAL) {printf("%s\n", Node->value);}

        if (Node->Type == TOKEN_EOF) {break;}
    }
}
