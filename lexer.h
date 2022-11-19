typedef struct LEXER_STRUCT {
    char *src;
    char c;

    unsigned int i;
} Lexer_T;

Lexer_T *InitializeLexer(char *src);

char LexerPeekOffset(Lexer_T *lexer, int _offset);

void LexerAdvanceWithId(Lexer_T *lexer);
void LexerAdvanceWithDigit(Lexer_T *lexer);

void LexerSkipWhitespace(Lexer_T *lexer);
void LexerGenerateAST(Lexer_T *lexer);
void LexerAdvanceCharacter(Lexer_T *lexer);
