#include "exp.h"

Exp::Exp(z3::expr _exp, std::string _text) 
: exp(_exp), text(_text) {}

Exp::~Exp() {}

Exp* Exp::negate() {
    exp = !exp;
    text = "not " + text;
    return this;    
}

Exp* Exp::brackets() {
    text = "(" + text + ")";
    return this;
}

Exp* Exp::apply(OP op, Exp* e) {
    std::string op_text;

    switch (op) {
        case OP_ADD: exp = exp +  e->exp; op_text = " + "; break;
        case OP_SUB: exp = exp -  e->exp; op_text = " - "; break;
        case OP_MUL: exp = exp *  e->exp; op_text = " * "; break;
        case OP_DIV: exp = exp /  e->exp; op_text = " / "; break;
        case OP_AND: exp = exp && e->exp; op_text = " and "; break;
        case OP_OR:  exp = exp || e->exp; op_text = " or "; break;
        case OP_EQ:  exp = exp == e->exp; op_text = " == "; break;
        case OP_LEQ: exp = exp <= e->exp; op_text = " <= "; break;
    }
    
    text = text + op_text + e->text;

    delete e;
    return this;
}

std::string Exp::to_string() {
    return text;
}
