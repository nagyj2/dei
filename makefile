# Modified from: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

#Compiler and Linker
CC          := gcc-11
SHELL				:= /bin/zsh

#The Target Binary Program
TARGET      := dei
TESTMAIN		:= deid

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
LIB         := -L/usr/local/Cellar/check/0.15.2/lib
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

BISONFLAGS 	:= -t
FLEXFLAGS 	:=

CHECKFLAGS	:= -lcheck

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
SOURCES     := $(shell find $(SRCDIR) -type f ! \( -name "*.l" -o -name "*.y" -o -name "check_*" \) )
OBJECTS     := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

TEST_SOURCES:= $(shell find $(SRCDIR) -type f ! \( -name "*.l" -o -name "*.y" -o -name "deimain.c" \))
TEST_OBJECTS:= $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(TEST_SOURCES:.$(SRCEXT)=.$(OBJEXT)))

BISON_H			:= $(addprefix $(INCDIR)/, $(BISON:y=tab.h))
BISON_C			:= $(addprefix $(SRCDIR)/, $(basename $(BISON)))

FLEX_C			:= $(addprefix $(SRCDIR)/, $(FLEX:l=lex.c))

#Defauilt Make
all: bison flex $(TARGET)

debug: CFLAGS += -D DEBUG
debug: all

bison: $(addprefix $(SRCDIR)/, $(BISON))
	bison $(BISONFLAGS) -b $(BISON_C) --defines=$(BISON_H) $^

flex: $(addprefix $(SRCDIR)/, $(FLEX))
	flex $(FLEXFLAGS) -o $(FLEX_C) $^

tests: $(TESTMAIN)

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

$(TESTMAIN): $(TEST_OBJECTS)
	$(CC) $(CHECKFLAGS) -o $(TARGETDIR)/$(TESTMAIN) $^ $(LIB)

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
.PHONY: all remake clean cleaner resources test
