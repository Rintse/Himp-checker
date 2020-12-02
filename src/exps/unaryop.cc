#include "exp.h"

UnaryOp::UnaryOp(UNOP _op, Exp* c)
: op(_op), child(c) {}

UnaryOp::~UnaryOp() { delete child; }

z3::expr UnaryOp::to_Z3(z3::context* c) {
    switch (op) {
        case OP_NEG: return !child->to_Z3(c);
        default: std::cerr << "Invalid operator" << std::endl;
    }
    return c->string_val("?ERROR?");
}

std::string UnaryOp::to_string() {
    std::string s = "( ";
    switch (op) {
        case OP_NEG: s += "not "; break;
    }
    return s + child->to_string() + " )";
}

bool UnaryOp::contains(std::string to_find) {
    return child->contains(to_find);
}

Exp* UnaryOp::copy() {
    return new UnaryOp(op, child->copy());
}

Exp* UnaryOp::substitute(std::string to_sub, std::string sub) {
    return new UnaryOp(op, child->substitute(to_sub, sub));
}
