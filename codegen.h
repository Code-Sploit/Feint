#include "asmlexer.h"

#include <stdint.h>
#include <stdlib.h>

#define u32 uint8_t

typedef struct BYTEBUFFER {
    u32 buffer[256];
} BYTEBUFFER;

BYTEBUFFER *NewBBuffer(void);

void AssembleCode(char *_fname);
