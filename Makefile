CC=gcc
CFLAGS=-c
GFLAGS=-g -c
BUILDDIR=build

all: feintc

feintc:
	mkdir -p build
	$(CC) $(CFLAGS) feint.c -o $(BUILDDIR)/feint.o
	$(CC) $(CFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(CFLAGS) ast.c -o $(BUILDDIR)/ast.o
	$(CC) $(CFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(CFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(CFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/main.o

clean:
	rm -f build/*
	rm -f feintc

debug:
	mkdir -p build
	$(CC) $(GFLAGS) feint.c -o $(BUILDDIR)/feint.o
	$(CC) $(GFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(GFLAGS) ast.c -o $(BUILDDIR)/ast.o
	$(CC) $(GFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) $(GFLAGS) parser.c -o $(BUILDDIR)/parser.o
	$(CC) $(GFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/token.o $(BUILDDIR)/parser.o $(BUILDDIR)/main.o

remake:
	make clean && make
