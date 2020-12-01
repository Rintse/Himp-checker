#include <iostream>
#include <z3++.h>
#include "node.h"

/* Extern functions */
int yyparse();
int yylex_destroy();

extern Node* tree;

int main() {
    yyparse();
    printTree(tree);
    verifyTree(tree);
}
