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
Node* popBlock(size_t start_line);

extern "C" {
    int yylex(void);
}
// Current line number
extern size_t line;
// AST
Node* tree = nullptr;
// Strings to relative operators
std::map<std::string, BINOP> rops = {
    {"=", OP_EQ}, {"!=", OP_NEQ}, 
    {"<", OP_LT}, {">", OP_GT},
    {"<=", OP_LEQ}, {">=", OP_GEQ}
};

// Store the commands in nested block statements
std::stack< std::vector<Node*> > comStack;
// Store the predicates in nested block statements
std::stack< std::vector<Exp*> > predStack;

%}

// Tokens
%start Program // Start symbol
%token BTRUE BFALSE SKIP NUM ID
%left ';' // Sequencing is left associative
%token ASSIGNOP WHILE DO IF THEN ELSE
%left IMP       // Material implication
%left OR        // Logic or
%left AND       // Logic and
%left NOT       // Logic negation 
%token ROP      // Relative operator
%left '+' '-'   // Arithmetic
%left '*' '/'   // Arithmetic

/* Types to pass between lexer, rules and actions */
%union {
  char* str;    // IDs
  int   nr;     // Number values
  size_t line;  // Line numbers
  Exp*  exp;    // Expressions
  Node* node;   // Nodes
}

%% 

Program:        Block { tree = $<node>1; } ;


Block:          {   // New block, go deeper into stack 
                    predStack.push(std::vector<Exp*>());
                    comStack.push(std::vector<Node*>());
                    $<line>$ = line;
                } 
                Predicate { addPred($<exp>2); } 
                ComGen { $<node>$ = popBlock($<line>1); }
                ;


WhileBlock:     {   // New block, go deeper into stack 
                    predStack.push(std::vector<Exp*>());
                    comStack.push(std::vector<Node*>());
                    $<line>$ = line;
                } 
                Command ';' ComGen2 { $<node>$ = popBlock($<line>1); } ;


ComGen2:        Predicate { addPred($<exp>1); } Command ';' 
                ComGen2 { $<node>$ = $<node>5; } | /* λ */ ;


ComGen:         Command ';' Predicate { addPred($<exp>3); } 
                ComGen { $<node>$ = $<node>5; } | /* λ */ ;


Command:        Id ASSIGNOP AExp {
                    addCom(new Assign($<exp>1, $<exp>3, line));
                }
                | { $<line>$ = line; } IF BExp THEN Block ELSE Block {
                    addCom(new IfElse($<exp>3, $<node>5, $<node>7, $<line>1));
                }
                | { $<line>$ = line; } WHILE BExp Invariant DO WhileBlock {
                    addCom(new While($<exp>3, $<exp>4, $<node>6, $<line>1));
                }
                | SKIP {
                    addCom(new Skip(line));
                }
                ;


BExp:           BTRUE           { $<exp>$ = new Bool(true); }
                | BFALSE        { $<exp>$ = new Bool(false); }
                // Boolean operators (seperate due to differing precedences)
                | BExp IMP BExp { $<exp>$ = new BinaryOp($<exp>1, OP_IMP, $<exp>3); }
                | BExp OR BExp  { $<exp>$ = new BinaryOp($<exp>1, OP_OR, $<exp>3); }
                | BExp AND BExp { $<exp>$ = new BinaryOp($<exp>1, OP_AND, $<exp>3); }
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
Id:             ID { 
                    $<exp>$ = new Var($<str>1); 
                    free($<str>1);
                };

%%

// Pops the stacks after the end of a block statement is found
Node* popBlock(size_t start_line) {
    Node* tmp = new Block(comStack.top(), predStack.top(), start_line);
    comStack.pop();
    predStack.pop();
    return tmp;
}

void addCom(Node* command) {
    comStack.top().push_back(command);
}

void addPred(Exp* predicate) {
    predStack.top().push_back(predicate);
}

static void yyerror(const char *s)
{
  fprintf(stderr, "%s", s);
}

int yywrap()
{
  return(1);
}
 
