#include "parser.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void BadSyntax(char *_msg, int _ret)
{
    printf("Critical error: ['%s']\n", _msg);

    exit(_ret);
}

int MatchExpr(char *_e1, char *_e2)
{
    if (strcmp(_e1, _e2) == 0) { return 1;}
    if (strcmp(_e1, _e2) != 0) { return 0;}

    return 0;
}

int MatchExprAny(char *_e)
{
    return 1;
}

Scope_T *InitializeScope(char *_name, Scope_Node *_Tokens[256])
{
    Scope_T *NewScope = calloc(1, sizeof(Scope_T));

    NewScope->_name    = _name;
    NewScope->_T_count = 0;

    for (int i = 0; i < 256; i++)
    {
        NewScope->Tokens[i] = _Tokens[i];
    }

    return NewScope;
}

Scope_Node *InitializeScopeNode(char *_type, char *_name, char *_oper, char *_value, char *_otype)
{
    Scope_Node *NewNode = calloc(1, sizeof(Scope_Node));

    NewNode->_type  = _type;
    NewNode->_name  = _name;
    NewNode->_oper  = _oper;
    NewNode->_value = _value;
    NewNode->_otype = _otype;

    return NewNode;
}

char *DeclareVariable(char *_name, char *_oper, char *_value, char *_type)
{
    char *declaration = calloc(1, sizeof(char));

    if (MatchExpr(_type, "str"))
    {
        char *template = "%s: db '%s', 0\n";

        sprintf(declaration, template, _name, _value);
    }
    else if (MatchExpr(_type, "int"))
    {
        char *template = "mov edx, %s\n";

        sprintf(declaration, template, _value);
    }

    return declaration;
}

Scope_T *ParserCompile(Lexer_T *lexer, int _debug)
{
    Scope_Node *_MainTokens[256];
    Token_Node *CurToken = LexerGetNextToken(lexer);

    int _TokensAdded = 0;

    /* Allocate token array */
    for (int i = 0; i < 256; i++)
    {
        _MainTokens[i] = calloc(1, sizeof(Scope_Node));
    }

    while (CurToken->Type != TOKEN_EOF)
    {
        if (CurToken->Type == TOKEN_ID)
        {
            if (MatchExpr(CurToken->_value, "construct"))
            {
                /* Next token should be function name */
                Token_Node *_fname = LexerGetNextToken(lexer);

                if (MatchExpr(_fname->_value, "main"))
                {
                    /* Main function */

                    if (_debug) {printf("Found function declaration: Name: '%s'\n", _fname->_value);}
                }

                while (!MatchExpr(CurToken->_value, "{"))
                {
                    CurToken = LexerGetNextToken(lexer);
                }

                CurToken = LexerGetNextToken(lexer);

                while (!MatchExpr(CurToken->_value, "}"))
                {
                    if (MatchExpr(CurToken->_value, "int"))
                    {
                        Token_Node *_type  = InitializeToken("int", TOKEN_INT);

                        /* Next token should be variable name */
                        Token_Node *_name  = LexerGetNextToken(lexer);
                        Token_Node *_oper  = LexerGetNextToken(lexer);
                        Token_Node *_value = LexerGetNextToken(lexer);

                        Scope_Node *_Node = InitializeScopeNode(_type->_value, _name->_value, _oper->_value, _value->_value, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("Found variable declaration: Type: '%s' Name: '%s' Operator: '%s' Value: '%s' Scope: '%s'\n", _type->_value, _name->_value, _oper->_value, _value->_value, _fname->_value);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "str"))
                    {
                        Token_Node *_type  = InitializeToken("str", TOKEN_INT);

                        /* Next token should be variable name */
                        Token_Node *_name  = LexerGetNextToken(lexer);
                        Token_Node *_oper  = LexerGetNextToken(lexer);
                        Token_Node *_value = LexerGetNextToken(lexer);

                        Scope_Node *_Node = InitializeScopeNode(_type->_value, _name->_value, _oper->_value, _value->_value, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("Found variable declaration: Type: '%s' Name: '%s' Operator: '%s' Value: '%s' Scope: '%s'\n", _type->_value, _name->_value, _oper->_value, _value->_value, _fname->_value);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "return"))
                    {
                        char *_ret = LexerGetNextToken(lexer)->_value;

                        Scope_Node *_Node = InitializeScopeNode("void", "return", " ", _ret, "STATEMENT");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("Found return statement: Returns: '%s'\n", _ret);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExprAny(CurToken->_value) == 0)
                    {
                        BadSyntax("Unknown type declaration outside of struct!", 1);
                    }

                    CurToken = LexerGetNextToken(lexer);
                }
            }

            if (MatchExpr(CurToken->_value, "int"))
            {
                BadSyntax("Integer declaration outside of struct!", 1);
            }

            if (MatchExpr(CurToken->_value, "str"))
            {
                BadSyntax("String declaration outside of struct!", 1);
            }

            if (MatchExprAny(CurToken->_value) == 0)
            {
                BadSyntax("Unknown type declaration outside of struct!", 1);
            }
        }

        CurToken = LexerGetNextToken(lexer);
    }

    Scope_T *_MainScope = InitializeScope("main", _MainTokens);

    _MainScope->_T_count = _TokensAdded;

    return _MainScope;
}
