#include "result.h"
#include <cstddef>
#include <z3++.h>
#include "node.h"

Result::Result(z3::solver* s) : solver(s) {
    res = z3::unsat; // Default to valid
}

Result& Result::check(Node* com) {
    res = solver->check(); 

    if(res != z3::unsat) { // error!
        back_trace.push(com);
    } else {
        solver->pop();
    }

    return *this;
}

Result& Result::log(Node *com) {
    back_trace.push(com);
    return *this;
}

bool Result::valid() {
    return res == z3::unsat;
}

void Result::print_backtrace() {
    size_t depth = back_trace.size();

    while(!back_trace.empty()) {
        if(back_trace.size() != depth) {
            std::cout << "In: ";
        } else {
            std::cout << "At: ";
        }

        back_trace.front()->log();
        std::cout << " (line " << back_trace.front()->get_line() << ")";
        std::cout << std::endl;

        back_trace.pop();
    }

    std::cout << std::endl;
}

void Result::print() {
    switch (res) {
        case z3::unsat:
            std::cout << "Success!" << std::endl;
            break;
        case z3::sat:
            std::cout << "Failed to prove hoare triple:" << std::endl;

            print_backtrace();

            std::cout << "Counterxample:" << std::endl
            << solver->get_model() << std::endl;
            break;
        default:
            std::cout << "Could not prove correctness" << std::endl;
    }
}
