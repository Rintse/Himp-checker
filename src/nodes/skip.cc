#include "node.h"
#include <cstddef>
#include <iomanip>

Skip::Skip(size_t _line) : Node(_line) {}

Skip::~Skip() {}

void Skip::print(size_t indent) {
    std::cout << gen_indent(indent) << "Skip\n"; 
}

void Skip::log() {
    std::cout << std::setw(LOG_WIDTH) << std::left << "Skip"; 
}

Result Skip::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    s->push();
    
    s->add(pre->to_Z3(c));
    s->add(!post->to_Z3(c));

    return Result(s).check(this);
}

