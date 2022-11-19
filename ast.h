#include "lexer.h"

#define NewIndex(arr) \
    ((sizeof(arr) / sizeof(arr[0])) + 1)

typedef struct AST_STRUCT {
    char *value;

    enum {
        TOKEN_ID,
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
        TOKEN_SEMICOLON,
        TOKEN_EOF
    } Type;
} AST_Node;

void InvalidSyntax(char syn, int _ret);

AST_Node *InitializeASTNode(char *value, int type);
AST_Node *ASTGetNextNode(Lexer_T *lexer);
