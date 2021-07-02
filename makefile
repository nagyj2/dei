
BISONFLAGS = -t

FLEXOUT = dei.lex.c

DEIFUNC = dei.lib.c

CFLAGS = -Wall -g -O2 -ansi -ll

debug: clean all

all: dei_bison dei_flex dei

dei_bison: dei.y
	bison -d $(BISONFLAGS) $^

dei_flex: dei.l
	flex -o $(FLEXOUT) $<

dei: dei.tab.c $(FLEXOUT) $(DEIFUNC)
	gcc $(CFLAGS) -o $@ $?

clean:
	rm -rf dei dei.dSYM dei.tab.* $(FLEXOUT)

# dei: dei.l dei.y dei.h dei.lib.c
# 	bison -d dei.y -t
# 	flex -o dei.lex.c dei.l
# 	gcc $(CFLAGS) -o $@ dei.tab.c dei.lex.c dei.lib.c
