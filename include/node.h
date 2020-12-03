// An collection of nodes to be used in abstract syntax trees for
// the language "himp"

#ifndef node_h
#define node_h

#define LOG_WIDTH 40

#include <cstddef>
#include <vector>
#include <z3++.h>
#include "exp.h"
#include "result.h"

// Abstract node class
class Node {
    public:
        Node(size_t _line) : line(_line) {}
        virtual ~Node() {}
    
        size_t get_line();

        virtual void print(size_t) = 0;
        virtual void log() = 0;

        virtual Result verify(
            Exp* pre, Exp* post, z3::context* c, z3::solver* s
        ) = 0;
        size_t line;
};

// Print a (sub)tree as indented text
void printTree(Node* root);
// Prints an expresiion as a predicate (indented text) 
void printPredicate(Exp* predicate, size_t indent);
// Helper function that generates whitespace
std::string gen_indent(size_t indent);
// Verify correctness of program tree
void verifyTree(Node* root);

// Represents block statement
// @member commands: the imp commands
// @member predicates: the hoare predicates surrounding the commands
class Block : public Node {
    public:
        Block(
            std::vector<Node*> commands, 
            std::vector<Exp*> predicates,
            size_t line
        );
        ~Block();
        
        void print(size_t indent) override;       
        void log() override;       
        Result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;
        Result verify(z3::context *c, z3::solver *s);

    private:
        std::vector<Node*> commands;
        std::vector<Exp*> predicates;
};

// Represents and if else (mandatory) statement
// @member bexp: A boolean Exp representing the condition
// @member if_body: A Block representing the body of the true-case
// @member else_body: A Block representing the body of the false-case 
class IfElse : public Node {
    public:
        IfElse(Exp* _bexp, Node* _if_body, Node* _else_body, size_t line);
        ~IfElse();

        void print(size_t indent) override;       
        void log() override;       
        Result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;

    private:
        Exp* bexp;
        Node* if_body;
        Node* else_body;
};

// Represents a while loop statement 
// @member bexp: a boolean Exp representing the condition
// @member body: a Block representing the body of the loop 
class While : public Node {
    public:
        While(Exp* _bexp, Exp* inv, Node* _body, size_t line);
        ~While();

        void print(size_t indent) override;       
        void log() override;       
        Result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;

    private:
        Exp* bexp;
        Exp* inv;
        Node* body;
};

// Represents an assignment expression
// @member id: a variable Exp representing the variable that is assigned to
// @member aexp: an arithmetic Exp
class Assign : public Node {
    public:
        Assign(Exp* id, Exp* aexp, size_t line);
        ~Assign();
        
        void print(size_t indent) override;       
        void log() override;       
        Result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;

    private:
        Exp* id;
        Exp* aexp;
};

// Represents an skip (empty) statement
class Skip : public Node {
    public:
        Skip(size_t line);
        ~Skip();
        
        void print(size_t indent) override;
        void log() override;       
        Result verify(
            Exp* pre, Exp* post, z3::context *c, z3::solver* s
        ) override;
};

#endif
