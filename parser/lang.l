%{
#include <stdio.h>
#include <string>
#include <z3++.h>
#include "../include/node.h"
#include "par.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

extern int fileno(FILE *);
size_t line = 1;
int yylex(void);

#if defined(__cplusplus)
}
#endif

%}

%option nounput


LETTER  [a-zA-Z]
DIGIT   [0-9]
ID      {LETTER}({LETTER}|{DIGIT})*
NUM     -?({DIGIT}{DIGIT}*)


%%
{NUM}           { yylval.nr = std::stoi(yytext, NULL, 10);
                  return NUM;
                }
if              { return IF; }
then            { return THEN; }
else            { return ELSE; }
while           { return WHILE; }
do              { return DO; }
false           { return BFALSE; } 
true            { return BTRUE; } 
skip            { return SKIP; }
and|"∧"         { return AND; }
or|"∨"          { return OR; }
not|"¬"         { return NOT; }
"->"|"→"        { return IMP; }
":="            { return ASSIGNOP; }
{ID}            { yylval.str = strdup(yytext);
                  return ID;
                }
"<="|">="|"<"|">"|"="|"!=" { 
                  yylval.str = strdup(yytext); 
                  return ROP; 
                }
[ \t]+          { /* Eat tabs */ }
"\n"            { line++; }
.               { return(yytext[0]); }
%%

