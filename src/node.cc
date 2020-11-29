#include "node.h"
#include "exp.h"
#include <cstddef>

#define TAB_SIZE 4

void printTree(Node* root) {
    root->print(0);
}

std::string gen_indent(size_t indent) {
    return std::string(indent * TAB_SIZE, ' ');
}

void printPredicate(Exp* predicate, size_t indent) {
    std::cout << gen_indent(indent) << "{ "
    << predicate->to_string() << " }" << std::endl;
}

Node::Node() {

}


Block::Block(std::vector<Node*> c, std::vector<Exp*> p)
: commands(c), predicates(p) {}

void Block::print(size_t indent) {
    std::cout << gen_indent(indent) << "Block:" << std::endl;

    int i;
    for(i = 0; i < commands.size(); i++) {
        printPredicate(predicates[i], indent+1);
        commands[i]->print(indent+1);
    }
    printPredicate(predicates[i], indent+1);
}

void Block::verify() {

}


IfElse::IfElse(Exp* _bexp, Node* _if_body, Node* _else_body)
: bexp(_bexp), if_body(_if_body), else_body(_else_body) {}

void IfElse::print(size_t indent) {
    std::cout << gen_indent(indent) << "If " 
    << bexp->to_string() << " then" << std::endl;
    if_body->print(indent+1);
    
    std::cout << gen_indent(indent) << "else" << std::endl;
    else_body->print(indent+1);
}

void IfElse::verify() {

}


While::While(Exp* _bexp, Node* _body) 
: bexp(_bexp), body(_body) {}

void While::print(size_t indent) {
    std::cout << gen_indent(indent) << "While " 
    << bexp->to_string() << " do" << std::endl;
    body->print(indent+1);
}

void While::verify() {

}


Assign::Assign(Exp* _id, Exp* _aexp)
: id(_id), aexp(_aexp) {}

void Assign::print(size_t indent) {
    std::cout << gen_indent(indent) << id->to_string() 
    << " := " << aexp->to_string() << std::endl;
}

void Assign::verify() {

}

