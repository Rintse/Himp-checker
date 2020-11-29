#ifndef node_h
#define node_h 

#include "z3++.h"

enum OP {
    OP_ADD, OP_SUB, OP_DIV, OP_MUL, OP_AND, OP_OR, OP_EQ, OP_LEQ
};

class Node {
    public:
        Node();
        ~Node();
};

class BlockNode {
    
};

class IDNode {
    IDNode(char _op);
};


class Exp : public Node {
    public:
        Exp(z3::expr);
        ~Exp();
        z3::expr exp;
        Exp* negate();
        Exp* apply(OP, Exp*);

};

class BExp : public Node {
    public:
        z3::expr exp;
};

class AExp : public Node {
    public:
        z3::expr exp;
};

#endif
