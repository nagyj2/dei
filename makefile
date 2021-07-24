# Modified from: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

#Compiler and Linker
CC          := gcc-11
SHELL				:= /bin/zsh

#Target Binary Programs
TARGET      := dei
TESTTARGET	:= deid

#Source code directories
APPDIR      := ./src/app
DATADIR			:= ./src/data
GENDIR			:= ./src/gen
TESTDIR			:= ./src/tests

#Resource directories
INCDIR      := ./inc
BUILDDIR    := ./obj
TARGETDIR   := ./bin
DOCDIR			:= ./docs

#Tool files
FLEX				:= lexer.l
BISON				:= parser.y

#Flags, Libraries and Includes TODO : separate flags
CFLAGS      := -Wall -g -std=c11 -lcunit
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

#Names of Flex and Bison output
BISON_H			:= $(addprefix $(INCDIR)/, $(BISON:y=tab.h))
BISON_C			:= $(addprefix $(GENDIR)/, $(basename $(BISON)))
BISON_OUT		:= $(addsuffix .output, $(addprefix $(BUILDDIR)/,$(basename $(BISON))))

FLEX_C			:= $(addprefix $(GENDIR)/, $(FLEX:l=lex.c))

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

#Automatically find all sources in src directory and construct object dependancies
#Application specific files
APPSOURCES	:= $(shell find $(APPDIR) -type f ! \( -name "*.output" -o -name "*.l" -o -name "*.y" \) )
APPOBJECTS	:= $(patsubst $(APPDIR)/%, $(BUILDDIR)/%, $(APPSOURCES:.c=.o))

#Support data files
DATASOURCES	:= $(shell find $(DATADIR) -type f -name "*.c" )
DATAOBJECTS	:= $(patsubst $(DATADIR)/%, $(BUILDDIR)/%, $(DATASOURCES:.c=.o))

#Test files
TESTSOURCES	:= $(shell find $(TESTDIR) -type f -name "*.c" )
TESTOBJECTS	:= $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(TESTSOURCES:.c=.o))

#Generated files
GENSOURCES	:= $(BISON_C) $(FLEX_C) 
GENOBJECTS	:= $(patsubst $(GENDIR)/%, $(BUILDDIR)/%, $(FLEX_C:.c=.o) $(addsuffix .tab.o, $(BISON_C)) )


#Defauilt Make
debug: CFLAGS += -D DEBUG
debug: bison flex $(TARGET)

tests: clean $(TESTTARGET)

all: bison flex $(TARGET) docs

#Parser
bison: $(addprefix $(APPDIR)/, $(BISON))
	bison $(BISONFLAGS) -b $(BISON_C) --defines=$(BISON_H) $^

#Lexer
flex: $(addprefix $(APPDIR)/, $(FLEX))
	flex $(FLEXFLAGS) -o $(FLEX_C) $^

#Remake
remake: cleaner all

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects, Docs and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -rf $(DOCDIR)/html
	@$(RM) -rf $(DOCDIR)/latex

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

#Create program
$(TARGET): $(DATAOBJECTS) $(GENOBJECTS) $(APPOBJECTS)
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Create test program
$(TESTTARGET): $(DATAOBJECTS) $(TESTOBJECTS)
	@mkdir -p $(TARGETDIR)
	$(CC) -o $(TARGETDIR)/$(TESTTARGET) $^ $(LIB)

#Compile App file
$(BUILDDIR)/%.o: $(APPDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(APPDIR)/$*.c > $(BUILDDIR)/$*.d
	@cp -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.o:|' < $(BUILDDIR)/$*.d.tmp > $(BUILDDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp

#Compile Data file
$(BUILDDIR)/%.o: $(DATADIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(DATADIR)/$*.c > $(BUILDDIR)/$*.d
	@cp -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.o:|' < $(BUILDDIR)/$*.d.tmp > $(BUILDDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp

#Compile Generated file
$(BUILDDIR)/%.o: $(GENDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(GENDIR)/$*.c > $(BUILDDIR)/$*.d
	@cp -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.o:|' < $(BUILDDIR)/$*.d.tmp > $(BUILDDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp

#Compile Test file
$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(TESTDIR)/$*.c > $(BUILDDIR)/$*.d
	@cp -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.o:|' < $(BUILDDIR)/$*.d.tmp > $(BUILDDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp

#Documentation
docs:
	doxygen Doxyfile

#Non-File Targets
.PHONY: all remake clean cleaner tests debug docs
