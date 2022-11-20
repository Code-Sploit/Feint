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

AST_Node *InitializeASTFunc(char *_src, char *_dest, char *_arg, int type)
{
    AST_Node *Node = calloc(1, sizeof(AST_Node));

    Node->value = _src;
    Node->_dest = _dest;
    Node->_arg  = _arg;

    Node->Type  = type;

    return Node;
}

AST_Node *ASTGetNextNode(Lexer_T *lexer)
{
    LexerSkipWhitespace(lexer);

    while (LexerPeekOffset(lexer, 1) != '\0')
    {
        //printf("S: %d A: %d D: %d\n", isspace(lexer->c), isalpha(lexer->c), isdigit(lexer->c));

        if (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t') {LexerAdvanceCharacter(lexer); continue;}

        if (isalpha(lexer->c) != 0) {return InitializeASTNode(LexerAdvanceWithId(lexer), TOKEN_ID);}
        if (isdigit(lexer->c) != 0) {return InitializeASTNode(LexerAdvanceWithDigit(lexer), TOKEN_DIGIT);}

        if (isspace(lexer->c) == 0 && isalpha(lexer->c) == 0 && isdigit(lexer->c) == 0)
        {
            switch (lexer->c)
            {
                case '!': {LexerAdvanceCharacter(lexer); return InitializeASTNode("!", TOKEN_EXCL);}
                case '@': {LexerAdvanceCharacter(lexer); return InitializeASTNode("@", TOKEN_AT);}
                case '#': {LexerAdvanceCharacter(lexer); return InitializeASTNode("#", TOKEN_HASHTAG);}
                case '$': {LexerAdvanceCharacter(lexer); return InitializeASTNode("$", TOKEN_DOLLAR);}
                case '%': {LexerAdvanceCharacter(lexer); return InitializeASTNode("%", TOKEN_PERCENT);}
                case '^': {LexerAdvanceCharacter(lexer); return InitializeASTNode("^", TOKEN_UP);}
                case '&': {LexerAdvanceCharacter(lexer); return InitializeASTNode("&", TOKEN_ZS);}
                case '*': {LexerAdvanceCharacter(lexer); return InitializeASTNode("*", TOKEN_STAR);}
                case '(': {LexerAdvanceCharacter(lexer); return InitializeASTNode("(", TOKEN_LPAREN);}
                case ')': {LexerAdvanceCharacter(lexer); return InitializeASTNode(")", TOKEN_RPAREN);}
                case '+': {LexerAdvanceCharacter(lexer); return InitializeASTNode("+", TOKEN_PLUS);}
                case '=': {LexerAdvanceCharacter(lexer); return InitializeASTNode("=", TOKEN_IS);}
                case '{': {LexerAdvanceCharacter(lexer); return InitializeASTNode("{", TOKEN_RBRACE);}
                case '}': {LexerAdvanceCharacter(lexer); return InitializeASTNode("}", TOKEN_LBRACE);}
                case ';': {LexerAdvanceCharacter(lexer); return InitializeASTNode(";", TOKEN_SEMICOLON);}

                case '"': {LexerAdvanceCharacter(lexer); return InitializeASTNode(LexerGetVal(lexer, '"'), TOKEN_VAL);}
                case '-': {
                    if (LexerPeekOffset(lexer, 1) != '>') {LexerAdvanceCharacter(lexer); return InitializeASTNode("-", TOKEN_MINUS);}

                    char *_src  = LexerGetFunctionSrc(lexer);
                    char *_dest = LexerGetFunctionDest(lexer);
                    char *_arg  = LexerGetFunctionArg(lexer);

                    return InitializeASTFunc(_src, _dest, _arg, TOKEN_BFUNC);
                }

                default: InvalidSyntax(lexer->c, 1);
            }
        }

        LexerAdvanceCharacter(lexer);
    }

    return InitializeASTNode("\0", TOKEN_EOF);
}
