LEX				= flex
YACC			= bison -y -t
YFLAGS			= --defines --debug --verbose
LOADLIBS		= -lfl -lm
INC_DIR			= include
SRC_DIR			= src
MAIN_DIR		= main
PARSER_DIR		= parser
SOURCES			= $(shell find $(SRC_DIR)/ -name '*.cc')
MAIN_SRC		= $(shell find $(MAIN_DIR)/ -name '*.cc')
OBJECTS			= $(SOURCES:.cc=.o)
MAIN_OBJ		= $(MAIN_SRC:.cc=.o)
DEPS 			= $(OBJECTS:.o=.d)
MAIN_DEPS		= $(MAIN_OBJ:.o=.d)
MAIN_TARGET		= h2smt
CC				= g++
CFLAGS			= -std=c++11 -g -Wall
Z3FLAGS			= -lz3
CPPFLAGS 		= $(addprefix -I, $(INC_DIR)) -pedantic


all: $(MAIN_TARGET)

$(MAIN_TARGET):  $(OBJECTS) $(MAIN_OBJ) parser/par.o parser/lex.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LOADLIBS) $(Z3FLAGS)
-include $(MAIN_DEPS) $(DEPS)

# PARSER 
parser/lex.o: parser/lex.c parser/par.h
parser/par.o: parser/par.c

parser/par.h parser/par.c: parser/lang.y
	$(YACC) $(YFLAGS) $< -o parser/par.c

parser/lex.c: parser/lang.l
	$(LEX) -o $@ $<

# Objects
$(MAIN_DIR)%.o: %.cc %.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $< $(LOADLIBS)

$(SRC_DIR)%.o: %.cc %.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $< $(LOADLIBS)

clean:
	rm -f $(OBJECTS) $(MAIN_OBJ) $(DEPS) $(MAIN_DEPS) $(MAIN_TARGET)
	rm -f parser/par.c parser/par.h parser/lex.c
	rm -f parser/*.o
	rm -f parser/par.output
