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
void addCom(Node*);
void addPred(Exp*);

extern "C" {
    int yylex(void);
}
extern int lineno;

Node* tree = nullptr;

// Strings to relative operators
std::map<std::string, BINOP> rops = {
    {"==", OP_EQ}, {"!=", OP_NEQ}, 
    {"<", OP_LT}, {">", OP_GT},
    {"<=", OP_LEQ}, {">=", OP_GEQ}
};

std::stack< std::vector<Node*> > comStack;
std::stack< std::vector<Exp*> > predStack;

%}

/* Start symbol */
%start Program

%token BTRUE BFALSE SKIP NUM ID
%left ';' // Sequencing is left associative
%token ASSIGNOP WHILE DO IF THEN ELSE
%left OR // Logic or
%left AND // Logic and
%left NOT // Logic negation 
%token ROP // Relative operator
%left '+' '-' // Arithmetic
%left '*' '/' // Arithmetic

/* Types to pass between lexer, rules and actions */
%union {
  char* str;
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
                ComGen { $<node>$ = $<node>4; } ;

WhileBlock:     {   // New block, go deeper into stack 
                    predStack.push(std::vector<Exp*>());
                    comStack.push(std::vector<Node*>());
                } 
                Command ';' ComGen2 { $<node>$ = $<node>4; } ;

ComGen2:        Predicate { addPred($<exp>1); } Command ';' 
                ComGen2 { $<node>$ = $<node>5; }
                | /* λ */ { // Final statement of block
                    $<node>$ = new Block(comStack.top(), predStack.top());
                    comStack.pop();
                    predStack.pop();
                }
                ;

ComGen:         Command 
                /*  Handled implicity by using a stack in the
                    Command productions */
                ';' 
                Predicate { addPred($<exp>3); } 
                ComGen { $<node>$ = $<node>5; }
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
                | WHILE BExp Invariant DO WhileBlock {
                    addCom(new While($<exp>2, $<exp>3, $<node>5));
                }
                | SKIP {
                    addCom(new Skip());
                }
                ;

BExp:           BTRUE           { $<exp>$ = new Bool(true); }
                | BFALSE        { $<exp>$ = new Bool(false); }
                // Boolean operators (seperate due to differing precedences)
                | BExp AND BExp { $<exp>$ = new BinaryOp($<exp>1, OP_AND, $<exp>3); }
                | BExp OR BExp  { $<exp>$ = new BinaryOp($<exp>1, OP_OR, $<exp>3); }
                | NOT BExp      { $<exp>$ = new UnaryOp(OP_NEG, $<exp>2); }
                // Relative operators
                | AExp ROP AExp { 
                    $<exp>$ = new BinaryOp($<exp>1, rops[$<str>2], $<exp>3); 
                    free($<str>2);
                }
                | '(' BExp ')'  { $<exp>$ = $<exp>2; }
                ;
  
AExp:           Id              { $<exp>$ = $<exp>1; } 
                | NUM           { $<exp>$ = new Integer($<nr>1); }
                // Arithmetic operators (seperate due to differing precedences)
                | AExp '+' AExp { $<exp>$ = new BinaryOp($<exp>1, OP_ADD, $<exp>3); } 
                | AExp '-' AExp { $<exp>$ = new BinaryOp($<exp>1, OP_SUB, $<exp>3); }
                | AExp '*' AExp { $<exp>$ = new BinaryOp($<exp>1, OP_MUL, $<exp>3); }
                | AExp '/' AExp { $<exp>$ = new BinaryOp($<exp>1, OP_DIV, $<exp>3); }
                | '(' AExp ')'  { $<exp>$ = $<exp>2; }
                ;

Predicate:      '{' BExp '}' { $<exp>$ = $<exp>2; } ;

Invariant:      '[' BExp ']' { $<exp>$ = $<exp>2; } ;

Id:              ID { $<exp>$ = new Var($<str>1); } ;

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

int yywrap()
{
  return(1);
}
 
