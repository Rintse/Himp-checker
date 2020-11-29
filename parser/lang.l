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

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif

int lineno = 1;
%}

%option nounput


LETTER  [a-zA-Z]
DIGIT   [0-9]
ID      {LETTER}({LETTER}|{DIGIT})*
NUM     {DIGIT}{DIGIT}*


%%
{NUM}           { yylval.nr = strtol(yytext, NULL, 10);
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
"<="            { return LEQ; }
"=="            { return EQ; }
and             { return AND; }
or              { return OR; }
not             { return NOT; } 
":="            { return ASSIGNOP; }
{ID}            { yylval.idStr = strdup(yytext);
                  return ID;
                }
\n              { lineno++; }
[ \t]+          { }
.               { return(yytext[0]); }
%%

