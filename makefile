
BISONFLAGS = -t

FLEXOUT = dei.lex.c

DEIFUNC = dei.lib.c

CFLAGS = -Wall -g -O2 -ansi -ll


all: dei_flex dei_bison dei

dei_bison: dei.y
	bison $(BISONFLAGS) -d $^

dei_flex: dei.l
	flex -o $(FLEXOUT) $<

dei: dei.tab.c $(FLEXOUT) $(DEIFUNC)
	gcc $(CFLAGS) -o $@ $?

clean:
	rm -rf dei dei.dSYM dei.tab.* dei.lex.c

# dei: dei.l dei.y dei.h dei.lib.c
# 	bison -d dei.y -t
# 	flex -o dei.lex.c dei.l
# 	gcc $(CFLAGS) -o $@ dei.tab.c dei.lex.c dei.lib.c
