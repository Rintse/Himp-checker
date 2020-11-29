#ifndef exp_h
#define exp_h

#include<z3++.h>

enum OP {
    OP_ADD, OP_SUB, OP_DIV, OP_MUL, OP_AND, OP_OR, OP_EQ, OP_LEQ
};

class Exp {
    public:
        z3::expr exp;
        
        Exp(z3::expr);
        ~Exp();
        
        Exp* negate();
        Exp* apply(OP, Exp*);
};

#endif
