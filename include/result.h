#ifndef result_h
#define result_h

#include <cstddef>
#include <stack>
#include <z3++.h>

class Exp;
class Node;

class VerificationResult {
    public:
        VerificationResult(z3::solver* s, Exp* pre, Node* com, Exp* post);

        void print();
        VerificationResult& check();

        z3::solver* solver;
        bool correct;

        std::stack<Exp*> pres;
        std::stack<Node*> coms;
        std::stack<Exp*> posts;
};

#endif
