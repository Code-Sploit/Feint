#include "lexer.h"

#define NewIndex(arr) \
    ((sizeof(arr) / sizeof(arr[0])) + 1)

typedef struct AST_STRUCT {
    char *value;
    char *_dest;
    char *_arg;

    enum {
        TOKEN_ID,
        TOKEN_VAL,
        TOKEN_DIGIT,
        TOKEN_EXCL,
        TOKEN_AT,
        TOKEN_HASHTAG,
        TOKEN_DOLLAR,
        TOKEN_PERCENT,
        TOKEN_UP,
        TOKEN_ZS,
        TOKEN_STAR,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_IS,
        TOKEN_SEMICOLON,
        TOKEN_BFUNC,
        TOKEN_EOF
    } Type;
} AST_Node;

void InvalidSyntax(char syn, int _ret);

AST_Node *InitializeASTNode(char *value, int type);
AST_Node *InitializeASTFunc(char *_src, char *_dest, char *_arg, int type);
AST_Node *ASTGetNextNode(Lexer_T *lexer);
