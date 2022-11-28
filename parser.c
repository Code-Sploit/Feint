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

Scope_Node *InitializeScopeNode(char *_type, char *_name, char *_oper, char *_value, char *_scope, char *_otype)
{
    Scope_Node *NewNode = calloc(1, sizeof(Scope_Node));

    NewNode->_type  = _type;
    NewNode->_name  = _name;
    NewNode->_oper  = _oper;
    NewNode->_value = _value;
    NewNode->_scope = _scope;
    NewNode->_otype = _otype;

    return NewNode;
}

Scope_T *ParserCompile(Lexer_T *lexer, int _debug)
{
    Scope_Node *_MainTokens[256];
    Token_Node *CurToken = LexerGetNextToken(lexer);

    char *_CurrentScope;

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
                else
                {
                    /* Other function */

                    if (_debug) {printf("Found function declaration: Name: '%s'\n", _fname->_value);}
                }

                while (!MatchExpr(CurToken->_value, "{"))
                {
                    CurToken = LexerGetNextToken(lexer);
                }

                CurToken = LexerGetNextToken(lexer);

                _CurrentScope = _fname->_value;

                while (!MatchExpr(CurToken->_value, "}"))
                {
                    if (MatchExpr(CurToken->_value, "int"))
                    {
                        Token_Node *_type  = InitializeToken("int", TOKEN_INT);

                        /* Next token should be variable name */
                        Token_Node *_name  = LexerGetNextToken(lexer);
                        Token_Node *_oper  = LexerGetNextToken(lexer);
                        Token_Node *_value = LexerGetNextToken(lexer);

                        Scope_Node *_Node = InitializeScopeNode(_type->_value, _name->_value, _oper->_value, _value->_value, _CurrentScope, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("Found variable declaration: Type: '%s' Name: '%s' Operator: '%s' Value: '%s' Scope: '%s'\n", _type->_value, _name->_value, _oper->_value, _value->_value, _fname->_value);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "str"))
                    {
                        Token_Node *_type  = InitializeToken("str", TOKEN_STR);

                        /* Next token should be variable name */
                        Token_Node *_name  = LexerGetNextToken(lexer);
                        Token_Node *_oper  = LexerGetNextToken(lexer);
                        Token_Node *_value = LexerGetNextToken(lexer);

                        Scope_Node *_Node = InitializeScopeNode(_type->_value, _name->_value, _oper->_value, _value->_value, _CurrentScope, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("Found variable declaration: Type: '%s' Name: '%s' Operator: '%s' Value: '%s' Scope: '%s'\n", _type->_value, _name->_value, _oper->_value, _value->_value, _fname->_value);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "printvar"))
                    {
                        Token_Node *_L_PAREN = LexerGetNextToken(lexer);
                        Token_Node *_NAME    = LexerGetNextToken(lexer);
                        Token_Node *_R_PAREN = LexerGetNextToken(lexer);

                        Scope_Node *_Node = InitializeScopeNode(" ", "printvar", " ", _NAME->_value, _CurrentScope, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound printvar() function call: Accessed variable: '%s' Scope: '%s'\n", _NAME->_value, _CurrentScope);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "return"))
                    {
                        char *_ret = LexerGetNextToken(lexer)->_value;

                        Scope_Node *_Node = InitializeScopeNode("void", "return", " ", _ret, _CurrentScope, "STATEMENT");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound return statement: Returns: '%s'\n\n", _ret);}

                        CurToken = LexerGetNextToken(lexer);

                        /* Break because return statement in main function */

                        break;
                    }

                    if (MatchExpr(CurToken->_value, "back"))
                    {
                        Scope_Node *_Node = InitializeScopeNode("void", "back", " ", " ", _CurrentScope, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound back statement: Back: '%s'\n\n", _CurrentScope);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "call"))
                    {
                        char *_ret = LexerGetNextToken(lexer)->_value;

                        Scope_Node *_Node = InitializeScopeNode("void", "call", " ", _ret, _CurrentScope, "EXPR");

                        _MainTokens[_TokensAdded] = _Node;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound function call: Calls: '%s'\n\n", _ret);}

                        CurToken = LexerGetNextToken(lexer);

                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "sum"))
                    {
                        /* Skip the LPAREN */

                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Token_Node *__A  = LexerGetNextToken(lexer); /* First variable */
                        Token_Node *__C1 = LexerGetNextToken(lexer); /* Comma token (1) */
                        Token_Node *__B  = LexerGetNextToken(lexer); /* Second variable */
                        Token_Node *__C2 = LexerGetNextToken(lexer); /* Comma token (2) */
                        Token_Node *__C  = LexerGetNextToken(lexer); /* Result variable */

                        /* Skip the RPAREN */
                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Scope_Node *_Summation = InitializeScopeNode("sum", __A->_value, "+", __B->_value, _CurrentScope, "ARG");

                        _Summation->_second_argument = __C->_value;

                        _MainTokens[_TokensAdded] = _Summation;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound summation: SYSLOG: 'Yes' Add: '%s' To: '%s' -> Console!\n", __B->_value, __A->_value);}

                        CurToken = LexerGetNextToken(lexer);
                        
                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "sub"))
                    {
                        /* Skip the LPAREN */

                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Token_Node *__A  = LexerGetNextToken(lexer); /* First variable */
                        Token_Node *__C1 = LexerGetNextToken(lexer); /* Comma token (1) */
                        Token_Node *__B  = LexerGetNextToken(lexer); /* Second variable */
                        Token_Node *__C2 = LexerGetNextToken(lexer); /* Comma token (2) */
                        Token_Node *__C  = LexerGetNextToken(lexer); /* Result variable */

                        /* Skip the RPAREN */
                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Scope_Node *_Subtraction = InitializeScopeNode("sub", __A->_value, "-", __B->_value, _CurrentScope, "ARG");

                        _Subtraction->_second_argument = __C->_value;

                        _MainTokens[_TokensAdded] = _Subtraction;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound subtraction: SYSLOG: 'Yes' Subtract: '%s' From: '%s' -> Console!\n", __B->_value, __A->_value);}

                        CurToken = LexerGetNextToken(lexer);
                        
                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "mul"))
                    {
                        /* Skip the LPAREN */

                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Token_Node *__A  = LexerGetNextToken(lexer); /* First variable */
                        Token_Node *__C1 = LexerGetNextToken(lexer); /* Comma token (1) */
                        Token_Node *__B  = LexerGetNextToken(lexer); /* Second variable */
                        Token_Node *__C2 = LexerGetNextToken(lexer); /* Comma token (2) */
                        Token_Node *__C  = LexerGetNextToken(lexer); /* Result variable */

                        /* Skip the RPAREN */
                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Scope_Node *_Multiplication = InitializeScopeNode("mul", __A->_value, "*", __B->_value, _CurrentScope, "ARG");

                        _Multiplication->_second_argument = __C->_value;

                        _MainTokens[_TokensAdded] = _Multiplication;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound multiplication: SYSLOG: 'Yes' Multiply: '%s' With: '%s' -> Console!\n", __B->_value, __A->_value);}

                        CurToken = LexerGetNextToken(lexer);
                        
                        continue;
                    }

                    if (MatchExpr(CurToken->_value, "div"))
                    {
                        /* Skip the LPAREN */

                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Token_Node *__A  = LexerGetNextToken(lexer); /* First variable */
                        Token_Node *__C1 = LexerGetNextToken(lexer); /* Comma token (1) */
                        Token_Node *__B  = LexerGetNextToken(lexer); /* Second variable */
                        Token_Node *__C2 = LexerGetNextToken(lexer); /* Comma token (2) */
                        Token_Node *__C  = LexerGetNextToken(lexer); /* Result variable */

                        /* Skip the RPAREN */
                        for (int i = 0; i < 1; i++)
                        {
                            CurToken = LexerGetNextToken(lexer);
                        }

                        Scope_Node *_Division = InitializeScopeNode("div", __A->_value, "/", __B->_value, _CurrentScope, "ARG");

                        _Division->_second_argument = __C->_value;

                        _MainTokens[_TokensAdded] = _Division;

                        _TokensAdded++;

                        if (_debug) {printf("\nFound division: SYSLOG: 'Yes' Divide: '%s' By: '%s' -> Console!\n", __B->_value, __A->_value);}

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
