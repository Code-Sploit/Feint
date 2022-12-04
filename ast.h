#include "lexer.h"

/* This is an AST_Node */
typedef struct AST_T {
    /* Cross-arguments */
    char *_fname;
    char *_vname;
    char *_vvalue;
    char *_rvalue;

    /* Define the type of the variable (cross argument) */
    enum {
        VARIABLE_TYPE_INT,
        VARIABLE_TYPE_STR
    } vtype;

    /* Define the type of the AST_Node */
    enum {
        AST_STATEMENT,
        AST_CONDITION,
        AST_DECLARATION,
        AST_FUNCTION
    } type;

    /* Define the lexer given type of the AST_Node */
    enum {
        LEXER_TOKEN_INIT,
        LEXER_TOKEN_ID,
        LEXER_TOKEN_VAL,
        LEXER_TOKEN_DIGIT,
        LEXER_TOKEN_INT,
        LEXER_TOKEN_STR,
        LEXER_TOKEN_EXCL,
        LEXER_TOKEN_AT,
        LEXER_TOKEN_HASHTAG,
        LEXER_TOKEN_DOLLAR,
        LEXER_TOKEN_PERCENT,
        LEXER_TOKEN_UP,
        LEXER_TOKEN_ZS,
        LEXER_TOKEN_STAR,
        LEXER_TOKEN_LBRACE,
        LEXER_TOKEN_RBRACE,
        LEXER_TOKEN_LPAREN,
        LEXER_TOKEN_RPAREN,
        LEXER_TOKEN_PLUS,
        LEXER_TOKEN_MINUS,
        LEXER_TOKEN_IS,
        LEXER_TOKEN_SEMICOLON,
        LEXER_TOKEN_BFUNC,
        LEXER_TOKEN_COMMA,
        LEXER_TOKEN_NULL,
        LEXER_TOKEN_EOF
    } ltype;

    /* Stable variable (cross-argument) */
    unsigned int _isstable;

    /* Is this the end of the branch? */
    unsigned int _branchc;
    unsigned int _eob;
    unsigned int _cc;

    /* Child objects to continue the branch. Remains unset when _eob = 1 */
    struct AST_T **children;
} AST_T;

AST_T *NewAST(int _type, int _eob);
AST_T *EmptyAST(void);
AST_T *GenerateASTTree(char *fdata);
