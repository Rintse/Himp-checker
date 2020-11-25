CC			= g++
LEX			= flex
YACC		= bison -y -t
YFLAGS		= --defines --debug --verbose
LOADLIBS	= -lfl -lm

all: hoare

hoare: lex.o par.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBS)

par.h par.c: lang.y
	$(YACC) $(YFLAGS) $< -o par.c

lex.c: lang.l
	$(LEX) -o $@ $<

lex.o: lex.c par.h
par.o: par.c

clean: 
	rm *.o
	rm par.output
	rm lex.c par.c par.h 
	rm hoare
