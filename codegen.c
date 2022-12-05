#include "codegen.h"

#include <linux/elf.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FILE *openf(const char *_fname, const char *_fmode)
{
    FILE *__ptr = fopen(_fname, _fmode);

    if (!__ptr) {printf("Error opening file: (%s) in mode: (%s)\n", _fname, _fmode); exit(1);}

    return __ptr;
}

Elf64_Ehdr *InitializeELFHeader()
{
    Elf64_Ehdr *hdr = calloc(1, sizeof(Elf64_Ehdr));

    hdr->e_ident[0] = 0x7f;
    hdr->e_ident[1] = 0x45;
    hdr->e_ident[2] = 0x4c;
    hdr->e_ident[3] = 0x46;
    hdr->e_ident[4] = 0x02;
    hdr->e_ident[5] = 0x01;
    hdr->e_ident[6] = 0x01;
    hdr->e_ident[7] = 0x00;
    hdr->e_ident[8] = 0x00;
    hdr->e_ident[9] = 0x00;
    hdr->e_ident[10] = 0x00;
    hdr->e_ident[11] = 0x00;
    hdr->e_ident[12] = 0x00;
    hdr->e_ident[13] = 0x00;
    hdr->e_ident[14] = 0x00;
    hdr->e_ident[15] = 0x00;

    hdr->e_type = 0x03;
    hdr->e_machine = 0x3e;
    hdr->e_version = 0x01;
    hdr->e_entry = 0x1130;		/* Entry point virtual address */
    hdr->e_phoff = 0x40;		/* Program header table file offset */
    hdr->e_shoff = 0x84d0;		/* Section header table file offset */
    hdr->e_flags = 0x00;
    hdr->e_ehsize = 0x40;
    hdr->e_phentsize = 0x38;
    hdr->e_phnum = 0x0d;
    hdr->e_shentsize = 0x40;
    hdr->e_shnum = 0x1f;
    hdr->e_shstrndx = 0x1e;

    return hdr;
}

ELFEND *InitializeELFEnd()
{
    ELFEND *hdr = calloc(1, sizeof(ELFEND));

    hdr->barr[0] = 0x06;
    hdr->barr[1] = 0x00;
    hdr->barr[2] = 0x00;
    hdr->barr[3] = 0x00;
    hdr->barr[4] = 0x04;
    hdr->barr[5] = 0x00;
    hdr->barr[6] = 0x00;
    hdr->barr[7] = 0x00;
    hdr->barr[8] = 0x40;
    hdr->barr[9] = 0x00;
    hdr->barr[10] = 0x00;
    hdr->barr[11] = 0x00;
    hdr->barr[12] = 0x00;
    hdr->barr[13] = 0x00;
    hdr->barr[14] = 0x00;
    hdr->barr[15] = 0x00;

    return hdr;
}

void WriteELFHeader(Elf64_Ehdr *hdr, ELFEND *ehdr, char* _fname)
{
    FILE *__ptr = openf(_fname, "wb");

    fwrite(hdr, sizeof(Elf64_Ehdr), 1, __ptr);
    fwrite(ehdr, sizeof(ELFEND), 1, __ptr);
    fclose(__ptr);
}

void AssembleCode(char *_fname)
{
    FILE *__ptr = openf(_fname, "rb");

    char *_fdata = calloc(1, sizeof(char));
    char *_fline = calloc(1, sizeof(char));

    size_t Length;
    ssize_t Read;

    while ((Read = getline(&_fline, &Length, __ptr)) != -1)
    {
        size_t _fline_len = strlen(_fline);
        size_t _fdata_len = strlen(_fdata);

        _fdata = realloc(_fdata, (_fdata_len + _fline_len + 1));

        strcat(_fdata, _fline);
    }

    ASMLexer_T *lexer = InitializeASMLexer(_fdata);

    ASMToken_Node *tok = ASMLexerGetNextToken(lexer);

    while (tok->Type != ASMToken_EOF)
    {
        if (tok->Type == ASMToken_ID)
        {
            /* Can either be 'mov', 'push', 'pop' or 'syscall' */

            if (MatchExpr(tok->_value, ASM_SYNTAX_MOV))
            {
                /* Next token should be the register! */
                ASMToken_Node *__reg = ASMLexerGetNextToken(lexer);

                /* Next token should be TOKEN_COMMA, which is irrelevant to us so let's skip it! */
                tok = ASMLexerGetNextToken(lexer);

                /* Next token should be the value! */
                ASMToken_Node *__val = ASMLexerGetNextToken(lexer);
                
                printf("MOV Instruction! MOVE: (%s) into (%s)\n", __val->_value, __reg->_value);
            }
            else if (MatchExpr(tok->_value, ASM_SYNTAX_PUSH))
            {
                printf("PUSH Instruction!\n");
            }
            else if (MatchExpr(tok->_value, ASM_SYNTAX_POP))
            {
                printf("POP Instruction!\n");
            }
            else if (MatchExpr(tok->_value, ASM_SYNTAX_SYSCALL))
            {
                printf("SYSCALL Instruction!\n");
            }
        }

        tok = ASMLexerGetNextToken(lexer);
    }

    char *_OFILE = "output";

    Elf64_Ehdr *hdr = InitializeELFHeader();
    ELFEND *ehdr = InitializeELFEnd();

    WriteELFHeader(hdr, ehdr, _OFILE);
}
