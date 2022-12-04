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

typedef struct ASMToken_STRUCT {
    char *_value;
    char *_dest;
    char *_arg;

    enum {
        ASMToken_INIT,
        ASMToken_ID,
        ASMToken_VAL,
        ASMToken_DIGIT,
        ASMToken_INT,
        ASMToken_STR,
        ASMToken_EXCL,
        ASMToken_AT,
        ASMToken_HASHTAG,
        ASMToken_DOLLAR,
        ASMToken_PERCENT,
        ASMToken_UP,
        ASMToken_ZS,
        ASMToken_STAR,
        ASMToken_LBRACE,
        ASMToken_RBRACE,
        ASMToken_LPAREN,
        ASMToken_RPAREN,
        ASMToken_PLUS,
        ASMToken_MINUS,
        ASMToken_IS,
        ASMToken_SEMICOLON,
        ASMToken_BFUNC,
        ASMToken_COMMA,
        ASMToken_NULL,
        ASMToken_EOF
    } Type;
} ASMToken_Node;

ASMToken_Node *InitializeASMToken(char *_value, int _type);
