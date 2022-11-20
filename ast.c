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
                case '!': {Nodes[NewIndex(Nodes)] = InitializeASTNode("!", TOKEN_EXCL); LexerAdvanceCharacter(lexer); continue;}
                case '@': {Nodes[NewIndex(Nodes)] = InitializeASTNode("@", TOKEN_AT); LexerAdvanceCharacter(lexer); continue;}
                case '#': {Nodes[NewIndex(Nodes)] = InitializeASTNode("#", TOKEN_HASHTAG); LexerAdvanceCharacter(lexer); continue;}
                case '$': {Nodes[NewIndex(Nodes)] = InitializeASTNode("$", TOKEN_DOLLAR); LexerAdvanceCharacter(lexer); continue;}
                case '%': {Nodes[NewIndex(Nodes)] = InitializeASTNode("%", TOKEN_PERCENT); LexerAdvanceCharacter(lexer); continue;}
                case '^': {Nodes[NewIndex(Nodes)] = InitializeASTNode("^", TOKEN_UP); LexerAdvanceCharacter(lexer); continue;}
                case '&': {Nodes[NewIndex(Nodes)] = InitializeASTNode("&", TOKEN_ZS); LexerAdvanceCharacter(lexer); continue;}
                case '*': {Nodes[NewIndex(Nodes)] = InitializeASTNode("*", TOKEN_STAR); LexerAdvanceCharacter(lexer); continue;}
                case '(': {Nodes[NewIndex(Nodes)] = InitializeASTNode("(", TOKEN_LPAREN); LexerAdvanceCharacter(lexer); continue;}
                case ')': {Nodes[NewIndex(Nodes)] = InitializeASTNode(")", TOKEN_RPAREN); LexerAdvanceCharacter(lexer); continue;}
                case '-': {Nodes[NewIndex(Nodes)] = InitializeASTNode("-", TOKEN_MINUS); LexerAdvanceCharacter(lexer); continue;}
                case '+': {Nodes[NewIndex(Nodes)] = InitializeASTNode("+", TOKEN_PLUS); LexerAdvanceCharacter(lexer); continue;}
                case '{': {Nodes[NewIndex(Nodes)] = InitializeASTNode("{", TOKEN_LBRACE); LexerAdvanceCharacter(lexer); continue;}
                case '}': {Nodes[NewIndex(Nodes)] = InitializeASTNode("}", TOKEN_RBRACE); LexerAdvanceCharacter(lexer); continue;}
                case ';': {Nodes[NewIndex(Nodes)] = InitializeASTNode(";", TOKEN_SEMICOLON); LexerAdvanceCharacter(lexer); continue;}

                case '"': {Nodes[NewIndex(Nodes)] = InitializeASTNode(LexerGetVal(lexer, "\""), TOKEN_VAL); LexerAdvanceCharacter(lexer); continue;}

                default: InvalidSyntax(lexer->c, 1);
            }
        }

        LexerAdvanceCharacter(lexer);
    }
}
