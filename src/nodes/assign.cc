#include "node.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <iomanip>
#include <z3++.h>

Assign::Assign(Exp* _id, Exp* _aexp, size_t _line)
: Node(_line), id(_id), aexp(_aexp) {}

Assign::~Assign() {
    delete id;
    delete aexp;
}

void Assign::print(size_t indent) {
    std::cout << gen_indent(indent) << id->to_string() 
    << " := " << aexp->to_string() << std::endl;
}

void Assign::log() {
    std::cout << std::setw(LOG_WIDTH) << std::left << 
    id->to_string() + " := " + aexp->to_string();
}

Result Assign::verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) {
    std::string id_str = id->to_string();

    s->push();
    s->add(pre->to_Z3(c));
    
    if(aexp->contains(id_str)) { // Self assignment: do [_x/x] in command and post
        std::unique_ptr<Exp> _id(new Var("_" + id_str)); 
        std::unique_ptr<Exp> _post(post->substitute(id_str, "_" + id_str));
        
        s->add(_id->to_Z3(c) == aexp->to_Z3(c));
        s->add(!_post->to_Z3(c));
    }
    else { //just use the original statement
        s->add(id->to_Z3(c) == aexp->to_Z3(c));
        s->add(!post->to_Z3(c));
    }

    return Result(s).check(this);
}
