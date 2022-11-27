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
	$(CC) $(CFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(CFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(CFLAGS) psyntax.c -o $(BUILDDIR)/psyntax.o
	$(CC) $(CFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/psyntax.o $(BUILDDIR)/main.o

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
	$(CC) $(GFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(GFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(CFLAGS) psyntax.c -o $(BUILDDIR)/psyntax.o
	$(CC) $(GFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/psyntax.o $(BUILDDIR)/main.o

compile:
	make clean && make
