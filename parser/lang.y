%{

#include <stdio.h>
#include <math.h>
#include <map>
#include <vector>
#include <stack>
#include <z3++.h>
#include "../include/node.h"

/* Prototypes */
static void yyerror(const char *);
Exp* constant(int);
Exp* constant(bool);
Exp* lookup(std::string);
void addCom(Node*);
void addPred(Exp*);

extern "C" {
    int yylex(void);
}
extern int lineno;

Node* tree;

z3::context context;
std::map<std::string, z3::expr> z3_vars;
std::vector<z3::expr> z3_stack;
Exp* lastPred;

std::stack< std::vector<Node*> > comStack;
std::stack< std::vector<Exp*> > predStack;

%}

/* Start symbol */
%start Program

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
  char* idStr;
  int   nr;
  Exp*  exp;
  Node* node;
}

%% 

Program:        Block { tree = $<node>1; } ;

Block:          {   // New block, go deeper into stack 
                    predStack.push(std::vector<Exp*>());
                    comStack.push(std::vector<Node*>());
                } 
                Predicate { addPred($<exp>2); } 
                ComGen { $<node>$ = $<node>4; };

ComGen:         Command ';' 
                Predicate { 
                    addPred($<exp>3); 
                } ComGen
                | /* λ */ { // Final statement of block
                    $<node>$ = new Block(comStack.top(), predStack.top());
                    comStack.pop();
                    predStack.pop();
                }
                ;

Command:        Id ASSIGNOP AExp {
                    addCom(new Assign($<exp>1, $<exp>3));
                }
                | IF BExp THEN Block ELSE Block {
                    addCom(new IfElse($<exp>2, $<node>4, $<node>6));
                }
                | WHILE BExp DO Block {
                    addCom(new While($<exp>2, $<node>4));
                }
                | SKIP {
                    addCom(new Skip());
                }
                ;

BExp:           BTRUE           { $<exp>$ = constant(true); }
                | BFALSE        { $<exp>$ = constant(false); }
                | AExp LEQ AExp { $<exp>$ = $<exp>1->apply(OP_LEQ, $<exp>3); }
                | AExp EQ AExp  { $<exp>$ = $<exp>1->apply(OP_EQ, $<exp>3); }
                | BExp AND BExp { $<exp>$ = $<exp>1->apply(OP_AND, $<exp>3); }
                | BExp OR BExp  { $<exp>$ = $<exp>1->apply(OP_OR, $<exp>3); }
                | NOT BExp      { $<exp>$ = $<exp>1->negate(); }
                | '(' BExp ')'  { $<exp>$ = $<exp>2; }
                ;
  
AExp:           Id              { $<exp>$ = $<exp>1; } 
                | NUM           { $<exp>$ = constant($<nr>1); }
                | AExp '+' AExp { $<exp>$ = $<exp>1->apply(OP_ADD, $<exp>3); } 
                | AExp '-' AExp { $<exp>$ = $<exp>1->apply(OP_SUB, $<exp>3); }
                | AExp '*' AExp { $<exp>$ = $<exp>1->apply(OP_MUL, $<exp>3); }
                | AExp '/' AExp { $<exp>$ = $<exp>1->apply(OP_DIV, $<exp>3); }
                | '(' AExp ')'  { $<exp>$ = $<exp>2; }
                ;

Predicate:      '{' BExp '}' { $<exp>$ = $<exp>2; } ;

Id:              ID { $<exp>$ = lookup($<idStr>1); } ;

%%

static void yyerror(const char *s)
{
  fprintf(stderr, "line %d: %s\n", lineno, s);
}

void addCom(Node* command) {
    comStack.top().push_back(command);
}

void addPred(Exp* predicate) {
    predStack.top().push_back(predicate);
}

Exp* constant(bool b) {
    return new Exp(context.bool_val(b));
}

Exp* constant(int c) {
    return new Exp(context.int_val(c));
}

Exp* lookup(std::string id) {
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
 
