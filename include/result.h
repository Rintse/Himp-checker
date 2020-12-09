// Class to represent the result of a translation from a hoare triple into 
// an SMT, and the verification of this SMT

#ifndef result_h
#define result_h

#include <queue>
#include <z3++.h>

class Node;

class Result {
    public:
        Result(z3::solver* s);
        
        // Checks solver, pushing com to the back_trace
        // in case the solver determines the triple is invalid
        Result& check(Node* com);

        // Checks the solver without pushing onto the back_trace
        // Used in "dummy"-checks (see block.cc for example)
        Result& check();
        
        // Simply pushes com onto the back_trace
        // used to pass error upwards and create back_trace
        Result& log(Node* com);

        bool valid();
        void print_backtrace();
        void print_counterexample();
        
        // Prints the validness of the result, 
        // and a back_trace if the result is invalid
        void print();

    private:
        z3::solver* solver;
        z3::check_result res;
        std::queue<Node*> back_trace;
};

#endif
