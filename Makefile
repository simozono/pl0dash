LDFLAGS=-ll

all: pl0-scanner parser00
clean:
	rm parser00 parser00.o
	rm pl0-scanner-for-parser.o
	rm pl0-scanner
parser00: parser00.o pl0-scanner-for-parser.o
parser00.o: parser00.c tokentable.h
pl0-scanner-for-parser.o: pl0-scanner-for-parser.l tokentable.h
