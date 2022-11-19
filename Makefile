CC=gcc
CFLAGS=-c
BUILDDIR=build

all: feintc

feintc:
	mkdir -p build
	$(CC) $(CFLAGS) feint.c -o $(BUILDDIR)/feint.o
	$(CC) $(CFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(CFLAGS) ast.c -o $(BUILDDIR)/ast.o
	$(CC) $(CFLAGS) main.c -o $(BUILDDIR)/main.o
	$(CC) -o feintc $(BUILDDIR)/feint.o $(BUILDDIR)/lexer.o $(BUILDDIR)/ast.o $(BUILDDIR)/main.o

clean:
	rm -f build/*
	rm -f feintc

remake:
	make clean && make
