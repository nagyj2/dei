# Modified from: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

#Compiler and Shell
CC          := gcc-11
SHELL				:= /bin/zsh

#Build Targets (Main Program, Unit Tests, Documentation)
TARGET      := dei
DEBUGTARGET := debug
TESTTARGET	:= tests
DOCUMENTS		:= docs

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
FLEX_L			:= lexer.l
BISON_Y			:= parser.y

FLEXOUT_C 	:= lexer.lex.c
BISONOUT_C 	:= parser.tab.c
BISONOUT_H 	:= $(BISONOUT_C:c=h)

#Flags, Libraries and Includes TODO : separate flags
CFLAGS      := -Wall -g -std=c11
FLEXFLAGS 	:=
BISONFLAGS 	:= -t
LIB         := 
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

#---------------------------------------------------------------------------------
#Automatic operations
#---------------------------------------------------------------------------------

#OS Specific flags
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OSS := -D LINUX
else #($(UNAME), Darwin)
OSS := -D OSX
endif

#Create path names of Flex and Bison output
GENFLEX_C		:= $(addprefix $(GENDIR)/, $(FLEXOUT_C))
GENBISON_C	:= $(addprefix $(GENDIR)/, $(BISONOUT_C))
GENBISON_H	:= $(addprefix $(INCDIR)/, $(BISONOUT_H))

#Automatically find all sources within src directory and construct object dependancies
#Main Application Specific Source Files
APPSOURCES	:= $(shell find $(APPDIR) -type f ! \( -name "*.output" -o -name "*.l" -o -name "*.y" \) )
APPOBJECTS	:= $(patsubst $(APPDIR)/%, $(BUILDDIR)/%, $(APPSOURCES:.c=.o))

#Support Data Source Files
DATASOURCES	:= $(shell find $(DATADIR) -type f -name "*.c" )
DATAOBJECTS	:= $(patsubst $(DATADIR)/%, $(BUILDDIR)/%, $(DATASOURCES:.c=.o))

#Test Source Files
TESTSOURCES	:= $(shell find $(TESTDIR) -type f -name "*.c" )
TESTOBJECTS	:= $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(TESTSOURCES:.c=.o))

#Generated Source Files
GENSOURCES	:= $(GENBISON_C) $(GENFLEX_C)
GENOBJECTS	:= $(patsubst $(GENDIR)/%, $(BUILDDIR)/%, $(GENSOURCES:.c=.o) )

#Create a debug version of main target
$(DEBUGTARGET): CFLAGS += -D DEBUG

#Create all artifacts
all: $(TESTTARGET) $(DEBUGTARGET) $(TARGET) $(DOCUMENTS)

#Remake all artifacts
remake: cleaner all

#Clean Object Directory
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean (Programs, Generated C Sources, Generated Documentation)
cleaner: clean
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -rf $(GENDIR)
	@$(RM) -rf $(DOCDIR)/html
	@$(RM) -rf $(DOCDIR)/latex

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

#Generate Lexer .c file
$(GENFLEX_C): $(addprefix $(APPDIR)/, $(FLEX_L))
	@mkdir -p $(GENDIR)
	flex $(FLEXFLAGS) -o $@ $?

#Generate Parser .c and .h files
$(GENBISON_C): $(addprefix $(APPDIR)/, $(BISON_Y))
	@mkdir -p $(GENDIR)
	bison $(BISONFLAGS) -o $@ --defines=$(GENBISON_H) $?

#Create main target program
$(TARGET): $(GENOBJECTS) $(DATAOBJECTS) $(APPOBJECTS)
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Create debug target program
$(DEBUGTARGET): $(GENOBJECTS) $(DATAOBJECTS) $(APPOBJECTS)
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(DEBUGTARGET) $^ $(LIB)

#Create test target program
$(TESTTARGET): $(DATAOBJECTS) $(TESTOBJECTS)
	@mkdir -p $(TARGETDIR)
	$(CC) -o $(TARGETDIR)/$(TESTTARGET) $^ $(LIB)

#Create Documentation
$(DOCUMENTS):
	doxygen Doxyfile

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




#Non-File Targets - always fully execute
.PHONY: all remake clean cleaner $(DOCUMENTS)
