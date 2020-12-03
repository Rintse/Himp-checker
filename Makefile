# Compilation tools
LEX				= flex
YACC			= bison -y -t
CC				= g++

# Directories
INC_DIR			= include
SRC_DIR			= src
MAIN_DIR		= main
PARSER_DIR		= parser

# Files
SOURCES			= $(shell find $(SRC_DIR)/ -name '*.cc')
MAIN_SRC		= $(shell find $(MAIN_DIR)/ -name '*.cc')
OBJECTS			= $(SOURCES:.cc=.o)
MAIN_OBJ		= $(MAIN_SRC:.cc=.o)
MAIN_TARGET		= h2smt

#Flags
CFLAGS			= -std=c++11 -g -Wall -pedantic
PARSE_FLAGS		= -lfl -lm
Z3FLAGS			= -lz3
YFLAGS			= --defines --debug --verbose
INCLUDE_FLAGS 	= $(addprefix -I, $(INC_DIR))


$(MAIN_TARGET):  $(OBJECTS) $(MAIN_OBJ) $(PARSER_DIR)/par.o $(PARSER_DIR)/lex.o
	$(CC) $(CFLAGS) -o $@ $^ $(PARSE_FLAGS) $(Z3FLAGS)

# PARSER 
$(PARSER_DIR)/lex.o: $(PARSER_DIR)/lex.c $(PARSER_DIR)/par.h $(OBJECTS)
$(PARSER_DIR)/par.o: $(PARSER_DIR)/par.c $(OBJECTS)

$(PARSER_DIR)/par.h $(PARSER_DIR)/par.c: $(PARSER_DIR)/lang.y
	$(YACC) $(YFLAGS) $< -o $(PARSER_DIR)/par.c

$(PARSER_DIR)/lex.c: $(PARSER_DIR)/lang.l
	$(LEX) -o $@ $<

# Objects
%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS) $(MAIN_OBJ) $(MAIN_TARGET)
	rm -f $(PARSER_DIR)/par.c $(PARSER_DIR)/par.h \
	$(PARSER_DIR)/lex.c $(PARSER_DIR)/par.output
	rm -f $(PARSER_DIR)/*.o
