%{

#include <stdio.h>
#include <math.h>
#include <map>
#include <vector>
#include "../include/node.h"
#include"z3++.h"

/* Prototypes */
static void yyerror(const char *);
Exp* constant(int);
Exp* constant(bool);
Exp* lookupVar(std::string);

extern "C" {
int yylex(void);   /* Lexer function */
}

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
  char*     idStr;
  int       nr;
  Exp*      exp;
  Node*     node;
}

%% 

Block:          Predicate ComGen Predicate ;

ComGen:         Command 
                | Command ';' Predicate {
                    
                }
                ComGen

Command:        ID { lookupVar($<idStr>1); }
                ASSIGNOP AExp {
                    
                }
                | IF BExp THEN Block ELSE Block
                | WHILE BExp DO Block
                | SKIP
                ;

BExp:             BTRUE         { $<exp>$ = constant(true); }
                | BFALSE        { $<exp>$ = constant(false); }
                | AExp LEQ AExp { $<exp>$ = $<exp>1->apply(OP_LEQ, $<exp>3); }
                | AExp EQ AExp  { $<exp>$ = $<exp>1->apply(OP_EQ, $<exp>3); }
                | BExp AND BExp { $<exp>$ = $<exp>1->apply(OP_AND, $<exp>3); }
                | BExp OR BExp  { $<exp>$ = $<exp>1->apply(OP_OR, $<exp>3); }
                | NOT BExp      { $<exp>$ = $<exp>1->negate(); }
                | '(' BExp ')'  { $<exp>$ = $<exp>2; }
                ;
  
AExp:             ID            { $<exp>$ = lookupVar($<idStr>1); } 
                | NUM           { $<exp>$ = constant($<nr>1); }
                | AExp '+' AExp { $<exp>$ = $<exp>1->apply(OP_ADD, $<exp>3); } 
                | AExp '-' AExp { $<exp>$ = $<exp>1->apply(OP_SUB, $<exp>3); }
                | AExp '*' AExp { $<exp>$ = $<exp>1->apply(OP_MUL, $<exp>3); }
                | AExp '/' AExp { $<exp>$ = $<exp>1->apply(OP_DIV, $<exp>3); }
                | '(' AExp ')'  { $<exp>$ = $<exp>2; }
                ;

Predicate:      '{' BExp '}' { $<exp>$ = $<exp>2; } ;

%%

static void yyerror(const char *s)
{
  fprintf(stderr, "line %d: %s\n", lineno, s);
}

Exp* constant(bool b) {
    return new Exp(context.bool_val(b));
}

Exp* constant(int c) {
    return new Exp(context.int_val(c));
}

Exp* lookupVar(std::string id) {
    return new Exp(
        (z3_vars.insert(
            std::make_pair(id, context.int_const(id.c_str()))
        ).first)->second
    );
}

int yywrap()
{
  return(1);
}
 
