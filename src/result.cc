#include "result.h"
#include <cstddef>
#include <iomanip>
#include <vector>
#include <z3++.h>
#include "node.h"

#define VALUE_WIDTH 20 // Decimal length of 2^64

Result::Result(z3::solver* s) : solver(s) {
    res = z3::unsat; // Default to valid
}

Result& Result::check(Node* com) {
    res = solver->check(); 

    if(res != z3::unsat) {
        // Invalid triple: push current command
        // onto the back_trace and don't pop
        // the solver to retain counterexample
        back_trace.push(com);
    } else { 
        // No error, clear solver
        solver->pop();
    }

    return *this;
}

Result& Result::check() {
    res = solver->check();
    // Dummy check, always pop
    // never push onto back_trace
    solver->pop();

    return *this;
}

Result& Result::log(Node *com) {
    back_trace.push(com);
    return *this;
}

bool Result::valid() {
    // A triple is valid if the solver couldn't find 
    // counterexamples to the negated post-condition
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

void Result::print_counterexample() {
    std::vector<std::string> ids;
    std::vector<z3::expr> values;

    z3::model m = solver->get_model();
    std::cout << "Counterxample:" << std::endl;

    unsigned longst_name = 0;

    // Gather ids and their values
    for(unsigned i = 0; i < m.num_consts(); i++) {
        ids.push_back(m[i]().to_string());
        values.push_back(m.get_const_interp(m[i]));

        // Keep track of the longest name for pretty printing
        if(ids.back().length() > longst_name)
            longst_name = ids.back().length();
    }

    // Print variables and their values
    for(size_t i = 0; i < ids.size(); i++) {
        std::cout
        << std::setw(longst_name) << std::left << ids[i] << " = " 
        << std::setw(VALUE_WIDTH) << std::left << values[i] << std::endl;
    }
}

void Result::print() {
    switch (res) {
        case z3::unsat:
            std::cout << "Success!" << std::endl;
            break;
        case z3::sat:
            std::cout << "Failed to prove hoare triple:" << std::endl;
            print_backtrace();
            print_counterexample();
            break;
        default:
            std::cout << "Could not prove correctness" << std::endl;
    }
}
