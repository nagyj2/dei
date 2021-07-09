
BISONFLAGS = -t -v

FLEXOUT = dei.lex.c

DEIFUNC = dei.lib.c

OBJS = struct.o symboltable.o evaluation.o util.o

CFLAGS = -Wall -g -std=c11 -ll

all: clean dei_bison dei_flex dei deid

dei_bison: dei.y
	bison $(BISONFLAGS) -d $^

dei_flex: dei.l
	flex -o $(FLEXOUT) $<

# release
dei: dei.tab.c $(FLEXOUT) $(DEIFUNC) $(OBJS)
	gcc $(CFLAGS) -O2 -o $@ $^

# debug
deid: dei.tab.c $(FLEXOUT) $(DEIFUNC) $(OBJS)
	@gcc $(CFLAGS) -D DEBUG -g -o $@ $^

clean:
	rm -rf dei deid dei.dSYM deid.dSYM dei.tab.* $(FLEXOUT)
