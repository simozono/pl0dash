CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison
all: pl0-scanner parser00 parser01 parser10 parser02
clean:
	rm -f parser00 parser00.o
	rm -f parser01 parser01.o
	rm -f parser02 parser02_bison.tab.c pl0-scanner-for-bison.o
	rm -f tokentable_bison.h
	rm -f parser10 parser10.o
	rm -f pl0-scanner-for-parser.o
	rm -f pl0-scanner.o
	rm -f pl0-scanner
parser10: parser10.o pl0-scanner-for-parser.o
parser10.o: parser10.c tokentable.h
parser00: parser00.o pl0-scanner-for-parser.o
parser00.o: parser00.c tokentable.h
parser01: parser01.o pl0-scanner-for-parser.o
parser01.o: parser01.c tokentable.h
parser02.o: parser02.c tokentable_bison.h
parser02: parser02.o parser02_bison.tab.c pl0-scanner-for-bison.o
pl0-scanner-for-parser.o: pl0-scanner-for-parser.l tokentable.h
pl0-scanner-for-bison.o: pl0-scanner-for-bison.l
pl0-scanner.o: pl0-scanner.l
parser02_bison.tab.c + tokentable_bison.h: parser02_bison.y
	$(YACC) --defines=tokentable_bison.h $<
