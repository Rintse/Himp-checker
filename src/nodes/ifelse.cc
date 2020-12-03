#include "exp.h"
#include "node.h"
#include <memory>
#include <z3++.h>

IfElse::IfElse(Exp* _bexp, Node* _if_body, Node* _else_body)
: bexp(_bexp), if_body(_if_body), else_body(_else_body) {}

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

z3::check_result IfElse::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    z3::check_result res;
    
    // Verify (bexp && if body) 
    std::unique_ptr<Exp> bexp_true(
        new BinaryOp(pre->copy(), OP_AND, bexp->copy())
    );
    if((res = if_body->verify(bexp_true.get(), post, c, s)) != z3::unsat)
        return res;
    
    // Verify (!bexp && else body) 
    std::unique_ptr<Exp> bexp_false(
        new BinaryOp(pre->copy(), OP_AND, new UnaryOp(OP_NEG, bexp->copy()))
    );
    res = else_body->verify(bexp_false.get(), post, c, s);

    return res;
}
