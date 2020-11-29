#include "node.h"

Node::Node() {}
Node::~Node() {}

Exp::Exp(z3::expr _exp) : exp(_exp) {}
Exp::~Exp() {}

Exp* Exp::negate() {
    exp = !exp;
    return this;    
}

Exp* Exp::apply(OP op, Exp* e) {
    switch (op) {
        case OP_ADD: exp = exp +  e->exp; 
        case OP_SUB: exp = exp -  e->exp; 
        case OP_MUL: exp = exp *  e->exp; 
        case OP_DIV: exp = exp /  e->exp;
        case OP_AND: exp = exp && e->exp;
        case OP_OR:  exp = exp || e->exp;
        case OP_EQ:  exp = exp == e->exp;
        case OP_LEQ: exp = exp <= e->exp;
    }

    delete e;
    return this;
}
