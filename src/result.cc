#include "result.h"
#include <cstddef>
#include <z3++.h>

VerificationResult::VerificationResult(
    z3::solver* s, Exp *pre, Node *com, Exp *post
) : solver(s) {
    pres.push(pre);
    coms.push(com);
    posts.push(post);
}

void VerificationResult::print() {
    std::cout << (correct ? "correct" : "incorrect") << std::endl;
}

VerificationResult& VerificationResult::check() {
    z3::check_result res = solver->check();

    if(res == z3::unsat) {
        correct = true;
        solver->pop();
    }
    else {
        correct = false;
    }

    return *this;
}
