%{

#include <stdio.h>
#include <math.h>
#include "../include/node.h"
#include"z3++.h"

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
%token ASSIGNOP WHILE DO IF THEN ELSE
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
  z3::expr *exp;
  Node* node;
}

%% 

Block:          Predicate ComGen Predicate ;

ComGen:         Command 
                | Command ';' Predicate ComGen

Command:        ID ASSIGNOP AExp
                | IF BExp THEN Block ELSE Block
                | WHILE BExp DO Block
                | SKIP
                ;

BExp:           BTRUE | BFALSE
                | AExp LEQ AExp
                | AExp EQ AExp
                | BExp AND BExp
                | BExp OR BExp
                | NOT BExp
                | '(' BExp ')' { $<node>$ = $<node>2; }
                ;

AExp:           ID { /*$<node>$ = new IDNode($<idStr>1);*/ } 
                | NUM { /*$<node>$ = new NumNode($<numStr>1); */ }
                | AExp '+' AExp { $<node>$ = new AOpNode('+', $<node>1, $<node>2); } 
                | AExp '-' AExp { $<node>$ = new AOpNode('-', $<node>1, $<node>2); }
                | AExp '*' AExp { $<node>$ = new AOpNode('*', $<node>1, $<node>2); }
                | AExp '/' AExp { $<node>$ = new AOpNode('/', $<node>1, $<node>2); }
                | '(' AExp ')' { $<node>$ = $<node>2; }
                ;

Predicate:      '{' BExp '}' { /*$<node>$ = new PredNode($<node>1);*/ } ;

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
 
