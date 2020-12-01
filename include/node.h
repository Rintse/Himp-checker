#ifndef node_h
#define node_h

#include <cstddef>
#include <vector>
#include <z3++.h>
#include "exp.h"

class Node {
    public:
        Node();
        virtual void print(size_t) {}        
        virtual z3::check_result verify(
            Exp* pre, Exp* post, z3::context* c, z3::solver* s
        ) = 0;
};

void printTree(Node* root);
std::string gen_indent(size_t indent);
void verifyTree(Node* root);

class Block : public Node {
    public:
        Block(std::vector<Node*> commands, std::vector<Exp*> predicates);
        
        std::vector<Node*> commands;
        std::vector<Exp*> predicates;
        
        void print(size_t indent) override;       
        z3::check_result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;
        z3::check_result verify(z3::context *c, z3::solver *s);

};


class IfElse : public Node {
    public:
        IfElse(Exp* _bexp, Node* _if_body, Node* _else_body);

        Exp* bexp;
        Node* if_body;
        Node* else_body;

        void print(size_t indent) override;       
        z3::check_result verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) override;
};


class While : public Node {
    public:
        While(Exp* _bexp, Node* _body);

        Exp* bexp;
        Node* body;

        void print(size_t indent) override;       
        z3::check_result verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) override;
};


class Assign : public Node {
    public:
        Assign(Exp* id, Exp* aexp);
        
        Exp* id;
        Exp* aexp;
        
        void print(size_t indent) override;       
        z3::check_result verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) override;
};


class Skip : public Node {
    public:
        Skip();
        
        void print(size_t indent) override;
        z3::check_result verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) override;
};

#endif
