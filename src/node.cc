#include "node.h"
#include "exp.h"

Node::Node() {

}

Block::Block(std::vector<Node*> c, std::vector<Exp*> p)
: commands(c), predicates(p) {}

void Block::verify() {

}


IfElse::IfElse(Exp* _bexp, Node* _if_body, Node* _else_body)
: bexp(_bexp), if_body(_if_body), else_body(_else_body) {}

void IfElse::verify() {

}


While::While(Exp* _bexp, Node* _body) 
: bexp(_bexp), body(_body) {}

void While::verify() {

}


Assign::Assign(Exp* _id, Exp* _aexp)
: id(_id), aexp(_aexp) {}

void Assign::verify() {

}


void Skip::verify() {

}

