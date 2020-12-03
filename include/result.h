#ifndef result_h
#define result_h

#include <queue>
#include <z3++.h>

class Node;

class Result {
    public:
        Result(z3::solver* s);
        
        Result& check(Node* com);
        Result& log(Node* com);
        bool valid();
        void print_backtrace();
        void print();

        z3::check_result res;
    private:
        std::queue<Node*> back_trace;
        z3::solver* solver;
};

#endif
