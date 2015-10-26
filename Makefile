CC = gcc
LEX = flex
LDLIBS = -ll
all: pl0-scanner parser00 parser10
clean:
	rm -f parser00 parser00.o
	rm -f parser10 parser10.o
	rm -f pl0-scanner-for-parser.o
	rm -f pl0-scanner.o
	rm -f pl0-scanner
parser10: parser10.o pl0-scanner-for-parser.o
parser10.o: parser10.c tokentable.h
parser00: parser00.o pl0-scanner-for-parser.o
parser00.o: parser00.c tokentable.h
pl0-scanner-for-parser.o: pl0-scanner-for-parser.l tokentable.h
pl0-scanner.o: pl0-scanner.l
