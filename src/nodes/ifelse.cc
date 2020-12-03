#include "exp.h"
#include "node.h"
#include <cstddef>
#include <memory>
#include <z3++.h>
#include <iomanip>

IfElse::IfElse(Exp* _bexp, Node* _if_body, Node* _else_body, size_t _line)
: Node(_line), bexp(_bexp), if_body(_if_body), else_body(_else_body) {}

IfElse::~IfElse() {
    delete bexp;
    delete if_body;
    delete else_body;
}

void IfElse::print(size_t indent) {
    std::cout << gen_indent(indent) << "If " 
    << bexp->to_string() << " then" << std::endl;
    if_body->print(indent+1);
    
    std::cout << gen_indent(indent) << "else" << std::endl;
    else_body->print(indent+1);
}

void IfElse::log() {
    std::cout << std::setw(LOG_WIDTH) << std::left <<
    "If " + bexp->to_string() + " then .. else ..";
}

Result IfElse::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    Result res(s);
    
    // Verify (bexp && if body) 
    std::unique_ptr<Exp> bexp_true(
        new BinaryOp(pre->copy(), OP_AND, bexp->copy())
    );
    if(!(res = if_body->verify(bexp_true.get(), post, c, s)).valid())
        return res.log(this);
    
    // Verify (!bexp && else body) 
    std::unique_ptr<Exp> bexp_false(
        new BinaryOp(pre->copy(), OP_AND, new UnaryOp(OP_NEG, bexp->copy()))
    );
    if(!(res = else_body->verify(bexp_false.get(), post, c, s)).valid())
        return res.log(this);

    return res;
}
