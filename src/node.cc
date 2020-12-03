#include "node.h"
#include "exp.h"
#include <cstddef>
#include <memory>
#include <z3++.h>
#include "result.h"

size_t Node::get_line() {
    return line;
}

#define TAB_SIZE 4

void printTree(Node* root) {
    root->print(0);
    std::cout << std::endl;
}

std::string gen_indent(size_t indent) {
    return std::string(indent * TAB_SIZE, ' ');
}

void printPredicate(Exp* predicate, size_t indent) {
    std::cout << gen_indent(indent) << "{ "
    << predicate->to_string() << " }" << std::endl;
}

void verifyTree(Node* root) {
    z3::context c;
    z3::solver s(c);

    dynamic_cast<Block*>(root)->verify(&c, &s).print(); 
}

