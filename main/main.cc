#include <iostream>
#include <z3++.h>
#include "node.h"

/* Extern functions */
int yyparse();
int yylex_destroy();

extern Node* tree;

int main() {
    yyparse();

    if(tree == nullptr) {
        return 1;
    }
    
    printTree(tree);
    verifyTree(tree);

    return 0;
}
