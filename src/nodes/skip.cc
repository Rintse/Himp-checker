#include "node.h"

Skip::Skip() {}

void Skip::print(size_t indent) {
    std::cout << gen_indent(indent) << "skip\n"; 
}

z3::check_result Skip::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    s->push();
    
    s->add(pre->to_Z3(c));
    s->add(!post->to_Z3(c));

    z3::check_result res = s->check();

    if(res == z3::unsat) s->pop();

    return res;
}

