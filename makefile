
CFLAGS = -Wall -g -O2 -ansi -ll

dei: dei.l dei.y dei.h dei.lib.c
	bison -d dei.y
	flex -o dei.lex.c dei.l
	gcc $(CFLAGS) -o $@ dei.tab.c dei.lex.c dei.lib.c


clean:
	rm -rf dei dei.dSYM dei.tab.* dei.lex.c
