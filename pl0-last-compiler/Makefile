CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison

EXT_TOKENTABLE = ../lexer/tokentable.h

all: pl0-compiler01
clean:
	rm -f pl0-compiler01 pl0-compiler01.o
	rm -f symbol_table.o misc.o codegen.o
	rm -f pl0-scanner-ll.c pl0-scanner-ll.o

pl0-compiler01: pl0-compiler01.o symbol_table.o pl0-scanner-ll.o misc.o codegen.o
symbol_table.o: symbol_table.c symbol_table.h
codegen.o:codegen.c codegen.h
pl0-compiler01.o: pl0-compiler01.c symbol_table.h codegen.h  ../pl0-parser/misc.h
misc.o: ../pl0-parser/misc.c ../pl0-parser/misc.h 
	$(CC) $(CFLAGS) -c -o $@ $<
pl0-scanner-ll.o: pl0-scanner-ll.c $(EXT_TOKENTABLE)
	$(CC) $(CFLAGS) -c -o $@ $<
pl0-scanner-ll.c: ../pl0-parser/pl0-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@
