#ifndef exp_h
#define exp_h

#include <cstddef>
#include<z3++.h>

enum OP {
    OP_ADD, OP_SUB, OP_DIV, OP_MUL, OP_AND, OP_OR, OP_EQ, OP_LEQ
};

class Exp {
    public:
        z3::expr exp;
        std::string text;
        
        Exp(z3::expr, std::string);
        ~Exp();
        
        Exp* negate();
        Exp* brackets();
        Exp* apply(OP, Exp*);

        std::string to_string();
};

#endif
