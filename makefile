# Modified from: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

#Compiler and Linker
CC          := gcc-11

#The Target Binary Program
TARGET      := program

TARGET      := program
TARGET      := program

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := ./src
INCDIR      := ./inc
BUILDDIR    := ./obj
TARGETDIR   := ./bin
SRCEXT      := c
OBJEXT      := o

FLEX				:= lexer.l
BISON				:= parser.y

#Flags, Libraries and Includes
CFLAGS      := -Wall -g -std=c11
LIB         :=
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

BISONFLAGS 	:= -t
FLEXFLAGS 	:=

#OS Specific flags
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OSS := -lfl -D LINUX
else #($(UNAME), Darwin)
OSS := -ll -D OSX
endif

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
DEPEXT      := d
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

BISON_H			:= $(addprefix $(INCDIR)/, $(BISON:y=tab.h))
BISON_C			:= $(addprefix $(SRCDIR)/, $(basename $(BISON)))

FLEX_C			:= $(addprefix $(SRCDIR)/, $(FLEX:l=lex.c))

#Defauilt Make
all: bison flex $(TARGET)

bison: $(addprefix $(SRCDIR)/, $(BISON))
	bison $(BISONFLAGS) -b $(BISON_C) --defines=$(BISON_H) $^

flex: $(addprefix $(SRCDIR)/, $(FLEX))
	flex $(FLEXFLAGS) -o $(FLEX_C) $^

#Remake
remake: cleaner all

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources

#
# CC = gcc-11
# CFLAGS := -Wall -g -std=c11
#
# #OS Specific flags
# UNAME := $(shell uname)
# ifeq ($(UNAME), Linux)
# OSS := -lfl -D LINUX
# else #($(UNAME), Darwin)
# OSS := -ll -D OSX
# endif
#
# DIR_OBJ += ./bin/
#
# DIR_SRC += ./src/
#
# DIR_INC += -I./inc/
# DIR_INC += $(addprefix -I, $(DIR_SRC))
#
# # Find all .c files in DIR_SRC
# SRC_C += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
# # Require .o from all .c
# OBJS += $(patsubst %.c, %.c, $(SRC_C))
#
# BISONFLAGS := -t
# BISONOUT = dei # prefix
#
# FLEXFLAGS :=
# FLEXOUT = dei.lex.c
#
#
#
# OBJS = deimain.o struct.o #symboltable.o evaluation.o util.o
#
# NAME = dei
#
#
# all: print $(OBJS)
#
# print:
# 	#echo $(OBJS)
#
# #exec: bison flex debug_exec
#
# #debug: CFLAGS += -D DEBUG
# #debug: BISONFLAGS += -v
# #debug: FLEXFLAGS += -d
# #debug: bison flex debug_exec
#
# #bison: parser.y
# #	bison $(BISONFLAGS) -b $(BISONOUT) -d $^
#
# #flex: lexer.l
# #	flex $(FLEXFLAGS) -o $(FLEXOUT) $<
#
# # Release
# #release_exec: dei.tab.c $(FLEXOUT) $(OBJS)
# #	$(CC) $(CFLAGS) $(OSS) -O2 -o $(EXEC) $?
#
# # Debug Options
# #debug_exec: dei.tab.c $(FLEXOUT) $(OBJS)
# #	@$(CC) $(CFLAGS) $(OSS) -o $(DEXEC) $?
#
# # Create object files
# $(DIR_OBJ)/%.o: $(DIR_SRC)%.c
# 	$(CC) $(CFLAG) $(DIR_INC) -c -o $@ $<
#
# clean:
# 	rm -rf $(EXEC) $(DEXEC) dei.output $(EXEC).dSYM $(DEXEC).dSYM dei.tab.c $(FLEXOUT) $(OBJS) *.h.gch
