
BISONFLAGS = -t

FLEXOUT = dei.lex.c

DEIFUNC = dei.lib.c

CFLAGS = -Wall -g -O2 -ansi -ll

all: clean dei_bison dei_flex dei deid

dei_bison: dei.y
	bison $(BISONFLAGS) -d $^

dei_flex: dei.l
	flex -o $(FLEXOUT) $<

# release
dei: dei.tab.c $(FLEXOUT) $(DEIFUNC)
	gcc $(CFLAGS) -o $@ $^

# debug
deid: dei.tab.c $(FLEXOUT) $(DEIFUNC)
	@gcc $(CFLAGS) -D DEBUG -g -o $@ $^

clean:
	rm -rf dei deid dei.dSYM deid.dSYM dei.tab.* $(FLEXOUT)
