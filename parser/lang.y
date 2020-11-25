%{

#include <stdio.h>
#include <math.h>

/* Prototypes */
static void yyerror(const char *);


/* Import from comp.l */
#ifdef __cplusplus
extern "C" {
#endif

int yylex(void);   /* Lexer function */

#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number */

%}

/* Start symbol */
%start Block

%token BTRUE BFALSE SKIP NUM ID
%left ';'
%token ASSIGNOP WHILE DO OD IF THEN ELSE FI
%left OR
%left AND
%left NOT 
%token LEQ EQ
%left '+' '-'
%left '*' '/'

/* Types to pass between lexer, rules and actions */
%union {
  char *idStr;
  char *numStr;
}

%% 

Block:          Predicate ComGen Predicate ;

ComGen:         Command 
                | Command ';' Predicate ComGen

Command:        ID ASSIGNOP AExp
                | IF BExp THEN Block ELSE Block FI
                | WHILE BExp DO Block OD
                | SKIP
                ;

BExp:           BTRUE | BFALSE
                | AExp LEQ AExp
                | AExp EQ AExp
                | BExp AND BExp
                | BExp OR BExp
                | NOT BExp
                | '(' BExp ')' 
                ;

AExp:           ID | NUM
                | AExp '+' AExp 
                | AExp '-' AExp 
                | AExp '*' AExp 
                | AExp '/' AExp 
                | '(' AExp ')'
                ;

Predicate:      '{' BExp '}' ;

%%

/* End of rules, more C code will follow now */

static void yyerror(const char *s)
{
  fprintf(stderr, "line %d: %s\n", lineno, s);
}

int yywrap()
{
  return(1);
}
 
