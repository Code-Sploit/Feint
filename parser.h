#include "lexer.h"

typedef struct SCOPE_NODE_STRUCT {
    char *_type;
    char *_name;
    char *_oper;
    char *_value;
    char *_scope;
    char *_otype;

    char *_second_argument;
} Scope_Node;

typedef struct SCOPE_STRUCT {
    char *_name;

    int _T_count;

    Scope_Node *Tokens[256];
} Scope_T;

Scope_T *ParserCompile(Lexer_T *lexer, int _debug);

Scope_T *InitializeScope(char *_name, Scope_Node **_Tokens);
Scope_Node *InitializeScopeNode(char *_type, char *_name, char *_oper, char *_value, char *_scope, char *_otype);
