#include <iostream>
#include "node.h"

/* Extern functions */
int yyparse();
int yylex_destroy();

extern Node* tree;

int main() {
    yyparse();
    printTree(tree);
}
