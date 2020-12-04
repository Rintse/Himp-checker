#include "exp.h"

BinaryOp::BinaryOp(Exp* c1, BINOP _op, Exp* c2) 
: op(_op), child1(c1), child2(c2) {}

BinaryOp::~BinaryOp() { delete child1; delete child2; }

z3::expr BinaryOp::to_Z3(z3::context* c) {
    switch (op) {
        case OP_ADD: return child1->to_Z3(c) +  child2->to_Z3(c);
        case OP_SUB: return child1->to_Z3(c) -  child2->to_Z3(c);
        case OP_MUL: return child1->to_Z3(c) *  child2->to_Z3(c);
        case OP_DIV: return child1->to_Z3(c) /  child2->to_Z3(c);
        case OP_AND: return child1->to_Z3(c) && child2->to_Z3(c);
        case OP_OR:  return child1->to_Z3(c) || child2->to_Z3(c);
        case OP_EQ:  return child1->to_Z3(c) == child2->to_Z3(c);
        case OP_NEQ: return child1->to_Z3(c) != child2->to_Z3(c);
        case OP_LT:  return child1->to_Z3(c) <  child2->to_Z3(c);
        case OP_GT:  return child1->to_Z3(c) >  child2->to_Z3(c);
        case OP_LEQ: return child1->to_Z3(c) <= child2->to_Z3(c);
        case OP_GEQ: return child1->to_Z3(c) >= child2->to_Z3(c);
        default: std::cerr << "Invalid operator" << std::endl;
    }
    return c->string_val("?ERROR?");
}

std::string BinaryOp::to_string() {
    std::string s = "( " + child1->to_string();
    switch (op) {
        case OP_ADD: s += " + ";   break;
        case OP_SUB: s += " - ";   break;
        case OP_MUL: s += " * ";   break;
        case OP_DIV: s += " / ";   break;
        case OP_AND: s += " and "; break;
        case OP_OR:  s += " or ";  break;
        case OP_EQ:  s += " == ";  break;
        case OP_NEQ: s += " != ";  break;
        case OP_LT:  s += " < ";   break;
        case OP_GT:  s += " > ";   break;
        case OP_LEQ: s += " <= ";  break;
        case OP_GEQ: s += " >= ";  break;
    }
    return s + child2->to_string() + " )";
}

bool BinaryOp::contains(std::string to_find) {
    return child1->contains(to_find) || child2->contains(to_find);
}

Exp* BinaryOp::copy() {
    return new BinaryOp(child1->copy(), op, child2->copy());
}

Exp* BinaryOp::substitute(std::string to_sub, std::string sub) {
    return new BinaryOp(
        child1->substitute(to_sub, sub),
        op,
        child2->substitute(to_sub, sub)
    );
}
