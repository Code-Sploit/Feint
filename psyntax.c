#include "psyntax.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void ThrowError(char *_tval, int _ind, int _ttype)
{
    char error_char = _tval[_ind];

    printf("Critical error: Index [%d] of line containing: [%s]!\n", _ind, _tval);
}

void _EMSG(char *_M, char *_E)
{
    printf("Expected '%s' before '%s'!\n", _M, _E);
}

void _WMSG(char *_W)
{
    printf("W: '%s'\n", _W);
}

void PSyntaxCheck(char *FName)
{
    FILE *fp = fopen(FName, "r");

    if (!fp)
    {
        printf("[PreSyntaxChecker] Failed to read '%s'\n", FName);

        exit(0);
    }

    char *FCon = calloc(1, sizeof(char));

    char *Line;

    size_t Length;
    ssize_t Read;

    int _e = 0;
    int _l = 0;
    int _w = 0;

    while ((Read = getline(&Line, &Length, fp)) != -1)
    {
        FCon = realloc(FCon, (strlen(FCon) + strlen(Line) + 1));

        strcat(FCon, Line);

        _l++;
    }

    Lexer_T *lexer = InitializeLexer(FCon);
    Token_Node *T = LexerGetNextToken(lexer);

    while (T->Type != TOKEN_EOF)
    {
        if (T->Type == TOKEN_ID)
        {
            if (MatchExpr(T->_value, "construct"))
            {
                /* This means we are expecting 2 more tokens before EOL */

                T = LexerGetNextToken(lexer);

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, i, T->Type);

                        _e++;
                    }
                }

                /* Check if next token is acctually a TOKEN_LBRACE */

                T = LexerGetNextToken(lexer);

                if (strcmp(T->_value, "{") != 0)
                {
                    _EMSG("{", T->_value);

                    _e++;
                }

                /* If the next token is TOKEN_RBRACE give a warning */
                
                T = LexerGetNextToken(lexer);

                if (strcmp(T->_value, "") == 0)
                {
                    T = LexerGetNextToken(lexer);
                }

                if (strcmp(T->_value, "}") == 0)
                {
                    _WMSG("FeintC forbids declaration of empty main function!");

                    _w++;
                }

                continue;
            }

            else if (MatchExpr(T->_value, "int") || MatchExpr(T->_value, "str"))
            {
                T = LexerGetNextToken(lexer);

                /* Process _vname */

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, i, T->Type);

                        _e++;
                    }
                }

                T = LexerGetNextToken(lexer);

                /* Should be a TOKEN_ASSIGN here */
                if (T->Type != TOKEN_IS)
                {
                    ThrowError(T->_value, 0, T->Type);

                    _e++;
                }

                T = LexerGetNextToken(lexer);

                /* Should be a value here */
                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }
            }
            else if (MatchExpr(T->_value, "sum") || MatchExpr(T->_value, "sub") || MatchExpr(T->_value, "mul") || MatchExpr(T->_value, "div"))
            {
                /* Skip TOKEN_LPAREN */
                for (int i = 0; i < 2; i++)
                {
                    T = LexerGetNextToken(lexer);
                }

                /* T should now have the value of the name of the first variable */
                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }

                /* Skip TOKEN_COMMA */
                for (int i = 0; i < 2; i++)
                {
                    T = LexerGetNextToken(lexer);
                }

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }

                /* Skip TOKEN_COMMA */

                for (int i = 0; i < 2; i++)
                {
                    T = LexerGetNextToken(lexer);
                }

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    int error = 0;

                    if (isalpha(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }
            }
            else if (MatchExpr(T->_value, "printvar"))
            {
                /* Skip TOKEN_LPAREN */
                for (int i = 0; i < 2; i++)
                {
                    T = LexerGetNextToken(lexer);
                }

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    char c = T->_value[i];

                    int error = 0;

                    if (isalpha(c) != 0)
                    {
                        error = 0;
                    }
                    else if (isdigit(T->_value[i]) != 0)
                    {
                        error = 0;
                    }
                    else
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }
            }
            else if (MatchExpr(T->_value, "return"))
            {
                T = LexerGetNextToken(lexer);

                for (int i = 0; i < strlen(T->_value); i++)
                {
                    char c = T->_value[i];

                    int error = 0;

                    if (isdigit(T->_value[i]) == 0)
                    {
                        error = 1;
                    }

                    if (error == 1)
                    {
                        ThrowError(T->_value, 0, T->Type);

                        _e++;
                    }
                }
            }
            else
            {
                ThrowError(T->_value, 0, T->Type);

                _e++;
            }
        }

        T = LexerGetNextToken(lexer);
    }

    printf("\nProcessed \033[1;36m[%d] Total Lines.\033[0m Found \033[1;31m[%d] Errors\033[0m and \033[1;33m[%d] Warnings\033[0m | Scanned File: \033[1;36m[%s]\033[0m!\n", _l, _e, _w, FName);

    if (_e > 0)
    {
        exit(0);
    }
}
