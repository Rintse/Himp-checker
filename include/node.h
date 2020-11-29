#ifndef node_h
#define node_h

#include <vector>
#include "exp.h"

class Node {
    public:
        Node();
        virtual void verify() {}
};


class Block : public Node {
    public:
        Block(std::vector<Node*> commands, std::vector<Exp*> predicates);
        
        std::vector<Node*> commands;
        std::vector<Exp*> predicates;
        
        void verify() override;

};


class IfElse : public Node {
    public:
        IfElse(Exp* _bexp, Node* _if_body, Node* _else_body);

        Exp* bexp;
        Node* if_body;
        Node* else_body;

        void verify() override;
};


class While : public Node {
    public:
        While(Exp* _bexp, Node* _body);

        Exp* bexp;
        Node* body;

        void verify() override;
};


class Assign : public Node {
    public:
        Assign(Exp* id, Exp* aexp);
        
        Exp* id;
        Exp* aexp;
        
        void verify() override;
};


class Skip : public Node {
    public:
        Skip() {}
        
        void verify() override;
};

#endif
