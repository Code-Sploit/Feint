#include "token.h"

typedef struct LEXER_STRUCT {
    char *src;
    char c;

    unsigned int i;
} Lexer_T;

int MatchExpr(char *_e1, char *_e2);

Lexer_T *InitializeLexer(char *src);

char LexerPeekOffset(Lexer_T *lexer, int _offset);

char *LexerAdvanceWithId(Lexer_T *lexer);
char *LexerAdvanceWithDigit(Lexer_T *lexer);

void LexerSkipWhitespace(Lexer_T *lexer);
void LexerAdvanceCharacter(Lexer_T *lexer);

Token_Node *LexerGetNextToken(Lexer_T *lexer);
Token_Node *LexerSkipToken(Lexer_T *lexer, int _count);
