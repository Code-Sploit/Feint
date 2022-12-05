#include "asmlexer.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct ELFEND_STRUCT {
    // 0600 0000 0400 0000 4000 0000 0000 0000

    uint8_t barr[16];
} ELFEND;

void AssembleCode(char *_fname);
