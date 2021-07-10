
CC = gcc
CFLAGS := -Wall -g -std=c11

#OS Specific flags
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OSS := -lfl -D LINUX
else #($(UNAME), Darwin)
OSS := -ll -D OSX
endif

BISONFLAGS := -t -v

FLEXOUT = dei.lex.c

OBJS = deimain.o struct.o symboltable.o evaluation.o util.o

EXEC = dei
DEXEC = dei

exec: parser lexer release_exec
debug: CFLAGS += -D DEBUG -g
debug:	parser lexer debug_exec

parser: dei.y
	bison $(BISONFLAGS) -d $^

lexer: dei.l
	flex -o $(FLEXOUT) $<


# Release
release_exec: dei.tab.c $(FLEXOUT) $(OBJS)
	$(CC) $(CFLAGS) $(OSS) -O2 -o $(EXEC) $?

# Debug Options
debug_exec: dei.tab.c $(FLEXOUT) $(MAIN) $(OBJS)
	@$(CC) $(CFLAGS) $(OSS) -o $(DEXEC) $?

# Create object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf dei deid dei.dSYM deid.dSYM dei.tab.* $(FLEXOUT) $(OBJS)
