CC=gcc
CFLAGS=-c
GFLAGS=-g -c
BUILDDIR=build

all: feintc

.PHONY: tools

feintc:
	mkdir -p build
	$(CC) $(CFLAGS) feint.c -o $(BUILDDIR)/feint.o
	$(CC) $(CFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(CFLAGS) ast.c -o $(BUILDDIR)/ast.o
	$(CC) $(CFLAGS) asm.c -o $(BUILDDIR)/asm.o
	$(CC) $(CFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(CFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(CFLAGS) codegen.c -o $(BUILDDIR)/codegen.o
	$(CC) $(CFLAGS) asmlexer.c -o $(BUILDDIR)/asmlexer.o
	$(CC) $(CFLAGS) asmtoken.c -o $(BUILDDIR)/asmtoken.o
	$(CC) $(CFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/asm.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/codegen.o $(BUILDDIR)/asmlexer.o $(BUILDDIR)/asmtoken.o $(BUILDDIR)/main.o

clean:
	rm -rf build
	rm -rf feintc
	rm -rf tools/test_compilation_process

tools:
	$(CC) -o tools/test_compilation_process tools/test_compilation_process.c

debug:
	mkdir -p build
	$(CC) $(GFLAGS) feint.c -o $(BUILDDIR)/feint.o
	$(CC) $(GFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(GFLAGS) ast.c -o $(BUILDDIR)/ast.o
	$(CC) $(GFLAGS) asm.c -o $(BUILDDIR)/asm.o
	$(CC) $(GFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(GFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(GFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/asm.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/main.o

compile:
	make clean && make
