#ifndef SYNTAXES
#define SYNTAXES

#define SYNTAX_FUNCTION_DEFINITION "create"
#define SYNTAX_INT_DEFINITION "int"
#define SYNTAX_STR_DEFINITION "string"
#define SYNTAX_STABLE_DEFINITION "stable"
#define SYNTAX_PRINT_CALL "print"
#define SYNTAX_RETURN_STATEMENT "ret"

#define PRIMARY_FUNCTION "main"

#endif

typedef struct TOKEN_STRUCT {
    char *_value;
    char *_dest;
    char *_arg;

    enum {
        TOKEN_INIT,
        TOKEN_ID,
        TOKEN_VAL,
        TOKEN_DIGIT,
        TOKEN_INT,
        TOKEN_STR,
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
        TOKEN_COMMA,
        TOKEN_NULL,
        TOKEN_EOF
    } Type;
} Token_Node;

Token_Node *InitializeToken(char *_value, int _type);
