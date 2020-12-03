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
    
    std::cout << "Program:" << std::endl;
    printTree(tree);

    std::cout << "Verifying correctness..." << std::endl;
    verifyTree(tree);

    delete tree;
    return 0;
}
