CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison

EXT_TOKENTABLE = ../lexer/tokentable.h

all: pl0-ll-parser01 pl0-ll-parser02 pl0-lr-parser01
clean:
	rm -f pl0-ll-parser01 pl0-ll-parser01.o
	rm -f pl0-ll-parser02 pl0-ll-parser02.o
	rm -f pl0-lr-parser01
	rm -f pl0-lr-parser_main.o
	rm -f pl0-lr-parser01_bison.tab.c
	rm -f pl0-lr-parser01_bison.tab.o
	rm -f tokentable_bison.h
	rm -f pl0-scanner-ll.c pl0-scanner-ll.o
	rm -f pl0-scanner-lr.c pl0-scanner-lr.o

pl0-ll-parser01: pl0-ll-parser01.o pl0-scanner-ll.o
pl0-ll-parser01.o: pl0-ll-parser01.c $(EXT_TOKENTABLE)

pl0-ll-parser02: pl0-ll-parser02.o pl0-scanner-ll.o
pl0-ll-parser02.o: pl0-ll-parser02.c $(EXT_TOKENTABLE)

pl0-lr-parser01: pl0-lr-parser_main.o pl0-lr-parser01_bison.tab.c pl0-scanner-lr.o
	        $(CC) $(CFLAGS) $^  $(LDLIBS) -o $@
pl0-lr-parser_main.o: pl0-lr-parser_main.c tokentable_bison.h

pl0-scanner-ll.o: pl0-scanner-ll.c $(EXT_TOKENTABLE)
	$(CC) $(CFLAGS) -c -o $@ $<
pl0-scanner-ll.c: pl0-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@

pl0-scanner-lr.o: pl0-scanner-lr.c tokentable_bison.h
	$(CC) $(CFLAGS) -DYACC -c -o $@ $<
pl0-scanner-lr.c: pl0-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@
pl0-lr-parser01_bison.tab.c + tokentable_bison.h: pl0-lr-parser01_bison.y
	$(YACC) --defines=tokentable_bison.h $<
