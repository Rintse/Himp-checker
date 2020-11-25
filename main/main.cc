#include <iostream>
#include "node.h"

/* Extern functions */
int yyparse();
int yylex_destroy();

int main() {
    yyparse();
    Node t;
}
