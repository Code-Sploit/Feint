#include "token.h"

#include <stdlib.h>

Token_Node *InitializeToken(char *_value, int _type)
{
    Token_Node *NewToken = calloc(1, sizeof(Token_Node));

    NewToken->_value = _value;
    NewToken->Type   = _type;

    return NewToken;
}
