%{

#include <stdio.h>
#include <math.h>
#include <map>
#include <vector>
#include "../include/node.h"
#include"z3++.h"

/* Prototypes */
static void yyerror(const char *);

z3::expr lookupVar(std::string);

/* Import from comp.l */
#ifdef __cplusplus
extern "C" {
#endif

int yylex(void);   /* Lexer function */

#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number */

z3::context context;
std::map<std::string, z3::expr> z3_vars;
std::vector<z3::expr> z3_stack;

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

Command:        ID { lookupVar($<idStr>1); }
                ASSIGNOP AExp {
                    
                }
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

AExp:           ID { lookupVar($<idStr>1); } 
                | NUM { context.int_val(strtol($<numStr>1, NULL, 10)); }
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

z3::expr lookupVar(std::string id) {
    return (z3_vars.insert(
        std::make_pair(id, context.int_const(id.c_str()))
    ).first)->second;
}

int yywrap()
{
  return(1);
}
 
