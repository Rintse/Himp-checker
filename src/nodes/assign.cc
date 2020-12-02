#include "node.h"

Assign::Assign(Exp* _id, Exp* _aexp)
: id(_id), aexp(_aexp) {}

void Assign::print(size_t indent) {
    std::cout << gen_indent(indent) << id->to_string() 
    << " := " << aexp->to_string() << std::endl;
}

z3::check_result Assign::verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) {
    if(aexp->contains(id)) {
    ...
    }
    return z3::unknown;
}
