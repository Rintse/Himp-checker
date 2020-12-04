#include "exp.h"
#include "node.h"
#include <cstddef>
#include <memory>
#include <z3++.h>
#include <iomanip>

While::While(Exp* _bexp, Exp* _inv, Node* _body, size_t _line) 
: Node(_line), bexp(_bexp), inv(_inv), body(_body) {}

While::~While() {
    delete bexp;
    delete inv;
    delete body;
}

void While::print(size_t indent) {
    std::cout << gen_indent(indent) << "While " 
    << bexp->to_string() << " [" << std::endl; 
    
    printPredicate(inv, indent+1);
    std::cout << gen_indent(indent) << "] do" << std::endl;
    
    body->print(indent+1);
}

void While::log() {
    std::cout << std::setw(LOG_WIDTH) << std::left << 
    "While " + bexp->to_string() + " do ..";
}

Result While::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    Result res(s);

    // Verify whether the invariant follows from the pre
    std::unique_ptr<Dummy> dummy(new Dummy);
    if(!(res = dummy->verify(pre, inv, c, s)).valid())
        return res.log(this);

    // Verify that the invariant holds during execution of body (bexp == true)
    std::unique_ptr<Exp> loop(
        new BinaryOp(inv->copy(), OP_AND, bexp->copy())
    );
    if(!(res = body->verify(loop.get(), inv, c, s)).valid())
        return res.log(this);

    // Verify that the invariant holds after execution of the loop (bexp == false)
    // and that the post condition follows
    std::unique_ptr<Exp> loop_end(
        new BinaryOp(inv->copy(), OP_AND, new UnaryOp(OP_NEG, bexp->copy()))
    );
    if(!(res = dummy->verify(loop_end.get(), post, c, s)).valid())
        return res.log(this);

    return res;
}

