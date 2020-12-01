#include "exp.h"
#include <string>

Exp::Exp() {}

Exp::~Exp() {}


UnaryOp::UnaryOp(UNOP _op, Exp* c)
: op(_op), child(c) {}

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

BinaryOp::BinaryOp(Exp* c1, BINOP _op, Exp* c2) 
: child1(c1), op(_op), child2(c2) {}

z3::expr BinaryOp::to_Z3(z3::context* c) {
    switch (op) {
        case OP_ADD: return child1->to_Z3(c) + child2->to_Z3(c);
        case OP_SUB: return child1->to_Z3(c) - child2->to_Z3(c);
        case OP_MUL: return child1->to_Z3(c) * child2->to_Z3(c);
        case OP_DIV: return child1->to_Z3(c) / child2->to_Z3(c);
        case OP_AND: return child1->to_Z3(c) && child2->to_Z3(c);
        case OP_OR:  return child1->to_Z3(c) || child2->to_Z3(c);
        case OP_EQ:  return child1->to_Z3(c) == child2->to_Z3(c);
        case OP_LEQ: return child1->to_Z3(c) <= child2->to_Z3(c);
        default: std::cerr << "Invalid operator" << std::endl;
    }
    return c->string_val("?ERROR?");
}

std::string BinaryOp::to_string() {
    std::string s = "( " + child1->to_string();
    switch (op) {
        case OP_ADD: s += " + "; break;
        case OP_SUB: s += " - "; break;
        case OP_MUL: s += " * "; break;
        case OP_DIV: s += " / "; break;
        case OP_AND: s += " and "; break;
        case OP_OR:  s += " or "; break;
        case OP_EQ:  s += " == "; break;
        case OP_LEQ: s += " <= "; break;
    }
    return s + child2->to_string() + " )";
}


Integer::Integer(int i) : val(i) {}

z3::expr Integer::to_Z3(z3::context* c) {
    return c->int_val(val);
}

std::string Integer::to_string() {
    return std::to_string(val);
}


Var::Var(std::string _id) : id(_id) {}

z3::expr Var::to_Z3(z3::context *c) {
    return c->int_const(id.c_str());
}

std::string Var::to_string() {
    return id;
}


Bool::Bool(bool b) : val(b) {}

z3::expr Bool::to_Z3(z3::context *c) {
    return c->bool_val(val);
}

std::string Bool::to_string() {
    return val ? "true" : "false";
}
