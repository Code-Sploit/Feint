#include "ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void InvalidSyntax(char syn, int _ret)
{
    printf("Critical error: ['%c'] -> undefined syntax.\n", syn);

    exit(_ret);
}

AST_Node *InitializeASTNode(char *value, int type)
{
    AST_Node *Node = calloc(1, sizeof(AST_Node));

    Node->value = value;
    Node->Type  = type;

    return Node;
}

AST_Node *ASTGetNextNode(Lexer_T *lexer)
{
    LexerSkipWhitespace(lexer);

    AST_Node *Nodes[256];

    for (int i = 0; i < 256; i++)
    {
        Nodes[i] = calloc(1, sizeof(AST_Node));
    }

    while (LexerPeekOffset(lexer, 1) != '\0')
    {
        //printf("S: %d A: %d D: %d\n", isspace(lexer->c), isalpha(lexer->c), isdigit(lexer->c));

        if (isspace(lexer->c) != 0) {LexerAdvanceCharacter(lexer); continue;}

        if (isalpha(lexer->c) != 0) {LexerAdvanceWithId(lexer);}
        if (isdigit(lexer->c) != 0) {LexerAdvanceWithDigit(lexer);}

        if (isspace(lexer->c) == 0 && isalpha(lexer->c) == 0 && isdigit(lexer->c) == 0)
        {
            switch (lexer->c)
            {
                case '!': {Nodes[NewIndex(Nodes)] = InitializeASTNode("!", TOKEN_EXCL);}
                case '@': {Nodes[NewIndex(Nodes)] = InitializeASTNode("@", TOKEN_AT);}
                case '#': {Nodes[NewIndex(Nodes)] = InitializeASTNode("#", TOKEN_HASHTAG);}
                case '$': {Nodes[NewIndex(Nodes)] = InitializeASTNode("$", TOKEN_DOLLAR);}
                case '%': {Nodes[NewIndex(Nodes)] = InitializeASTNode("%", TOKEN_PERCENT);}
                case '^': {Nodes[NewIndex(Nodes)] = InitializeASTNode("^", TOKEN_UP);}
                case '&': {Nodes[NewIndex(Nodes)] = InitializeASTNode("&", TOKEN_ZS);}
                case '*': {Nodes[NewIndex(Nodes)] = InitializeASTNode("*", TOKEN_STAR);}
                case '(': {Nodes[NewIndex(Nodes)] = InitializeASTNode("(", TOKEN_LPAREN);}
                case ')': {Nodes[NewIndex(Nodes)] = InitializeASTNode(")", TOKEN_RPAREN);}
                case '-': {Nodes[NewIndex(Nodes)] = InitializeASTNode("-", TOKEN_MINUS);}
                case '+': {Nodes[NewIndex(Nodes)] = InitializeASTNode("+", TOKEN_PLUS);}
                case '{': {Nodes[NewIndex(Nodes)] = InitializeASTNode("{", TOKEN_LBRACE);}
                case '}': {Nodes[NewIndex(Nodes)] = InitializeASTNode("}", TOKEN_RBRACE);}

                default: InvalidSyntax(lexer->c, 1);
            }
        }

        LexerAdvanceCharacter(lexer);
    }
}
