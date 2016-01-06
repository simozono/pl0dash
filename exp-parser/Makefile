CC = gcc
LEX = flex
LDLIBS = -ll
YACC = bison

EXT_TOKENTABLE = ../lexer/tokentable.h

all: exp-ll-parser01 exp-ll-parser02 exp-lr-parser01
clean:
	rm -f exp-ll-parser01 exp-ll-parser01.o
	rm -f exp-ll-parser02 exp-ll-parser02.o
	rm -f exp-lr-parser01 exp-lr-parser01.o
	rm -f exp-lr-parser_main.o
	rm -f exp-lr-parser01_bison.tab.c
	rm -f exp-lr-parser01_bison.tab.o
	rm -f tokentable_bison.h
	rm -f exp-scanner-ll.c exp-scanner-ll.o
	rm -f exp-scanner-lr.c exp-scanner-lr.o

exp-ll-parser01: exp-ll-parser01.o exp-scanner-ll.o
exp-ll-parser01.o: exp-ll-parser01.c $(EXT_TOKENTABLE)

exp-ll-parser02: exp-ll-parser02.o exp-scanner-ll.o
exp-ll-parser02.o: exp-ll-parser02.c $(EXT_TOKENTABLE)

exp-lr-parser01: exp-lr-parser_main.o exp-lr-parser01_bison.tab.c exp-scanner-lr.o
	        $(CC) $(CFLAGS) $^  $(LDLIBS) -o $@
exp-lr-parser_main.o: exp-lr-parser_main.c tokentable_bison.h

exp-scanner-ll.o: exp-scanner-ll.c $(EXT_TOKENTABLE)
	$(CC) $(CFLAGS) -c -o $@ $<
exp-scanner-ll.c: exp-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@

exp-scanner-lr.o: exp-scanner-lr.c tokentable_bison.h
	$(CC) $(CFLAGS) -DYACC -c -o $@ $<
exp-scanner-lr.c: exp-scanner.l
	$(LEX) $(LFLAGS) -t $< > $@
exp-lr-parser01_bison.tab.c + tokentable_bison.h: exp-lr-parser01_bison.y
	$(YACC) --defines=tokentable_bison.h $<
