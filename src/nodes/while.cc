#include "exp.h"
#include "node.h"
#include <memory>
#include <z3++.h>

While::While(Exp* _bexp, Exp* inv, Node* _body) 
: bexp(_bexp), invariant(inv), body(_body) {}

void While::print(size_t indent) {
    std::cout << gen_indent(indent) << "While " 
    << bexp->to_string() << " [" << std::endl; 
    printPredicate(invariant, indent+1);
    std::cout << gen_indent(indent) << "] do" << std::endl;
    body->print(indent+1);
}

z3::check_result While::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    z3::check_result res;

    // Verify whether the invariant follows from the pre
    std::unique_ptr<Node> dummy_skip(new Skip);
    if((res = dummy_skip->verify(pre, invariant, c, s)) != z3::unsat)
        return res;

    // Verify that the invariant holds during execution of body (bexp == true)
    std::unique_ptr<Exp> loop(
        new BinaryOp(invariant->copy(), OP_AND, bexp->copy())
    );
    if((res = body->verify(loop.get(), invariant, c, s)) != z3::unsat)
        return res;

    // Verify that the invariant holds after execution of the loop (bexp == false)
    // and that the post condition follows
    std::unique_ptr<Exp> loop_end(
        new BinaryOp(invariant, OP_AND, new UnaryOp(OP_NEG, bexp->copy()))
    );
    res = dummy_skip->verify(loop_end.get(), post, c, s);
   
    return res;
}

