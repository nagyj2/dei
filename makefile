
CC = gcc-11
CFLAGS := -Wall -g -std=c11

#OS Specific flags
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OSS := -lfl -D LINUX
else #($(UNAME), Darwin)
OSS := -ll -D OSX
endif

BISONFLAGS := -t
FLEXFLAGS :=

BISONOUT = dei # prefix
FLEXOUT = dei.lex.c

OBJS = deimain.o #struct.o symboltable.o evaluation.o util.o

EXEC = dei
DEXEC = dei

exec: bison flex release_exec

debug: CFLAGS += -D DEBUG
debug: BISONFLAGS += -v
debug: FLEXFLAGS += -d
debug: bison flex debug_exec

bison: parser.y
	bison $(BISONFLAGS) -b $(BISONOUT) -d $^

flex: lexer.l
	flex $(FLEXFLAGS) -o $(FLEXOUT) $<

# Release
release_exec: dei.tab.c $(FLEXOUT) $(OBJS)
	$(CC) $(CFLAGS) $(OSS) -O2 -o $(EXEC) $?

# Debug Options
debug_exec: dei.tab.c $(FLEXOUT) $(OBJS)
	@$(CC) $(CFLAGS) $(OSS) -o $(DEXEC) $?

# Create object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(EXEC) $(DEXEC) dei.output $(EXEC).dSYM $(DEXEC).dSYM dei.tab.c $(FLEXOUT) $(OBJS) *.h.gch
