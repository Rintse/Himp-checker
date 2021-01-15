#include <iostream>
#include <z3++.h>
#include "node.h"

/* Extern functions */
int yyparse();
int yylex_destroy();

/* The AST */
extern Node* tree;


int main() {
    yyparse();

    if(tree == nullptr) {
        return 1; // Parsing failed
    }
    
    std::cout << "Parsing succesfull. Syntax tree:" << std::endl;
    printTree(tree);

    std::cout << "Verifying correctness...\n" << std::endl;
    verifyTree(tree);

    delete tree;
    return 0;
}
