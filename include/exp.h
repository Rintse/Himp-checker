// A collection of expressions to be used int he abstract 
// syntax trees for the language "himp". Used in:
// Arithmetic expressions, hoare predicates, and boolean conditionals

#ifndef exp_h
#define exp_h

#include <cstddef>
#include<z3++.h>

// Binary operators
enum BINOP {
    OP_ADD, OP_SUB, OP_DIV, OP_MUL,             // arithmetic
    OP_AND, OP_OR, OP_IMP,                      // boolean
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LEQ, OP_GEQ // relative
};

// Unary operators
enum UNOP {
    OP_NEG
};

// Virtual class representing an arithmetic/boolean expression
class Exp {
    public:
        Exp() {}
        virtual ~Exp() {}
        
        // Returns a textual representation of this expression
        virtual std::string to_string() = 0;

        // Translates this expression into a z3::expr 
        virtual z3::expr to_Z3(z3::context* c) = 0;

        // Returns true if this expression contains the variable
        // with the name to_find. False otherwise.
        virtual bool contains(std::string to_find) = 0;
        
        // Returns a copy of this expression
        virtual Exp* copy() = 0;
        
        // Returns a copy of this expression, with each 
        // variable to_sub replaced with the variable sub
        virtual Exp* substitute(std::string to_sub, std::string sub) = 0;
};

// Unary operator (only negation is implemented here)
class UnaryOp : public Exp {
    public:
        UnaryOp(UNOP _op, Exp* c);
        ~UnaryOp();

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        Exp* copy() override;
        Exp* substitute(std::string to_sub, std::string sub) override;

    private:
        UNOP op;
        Exp* child;
};

// Binary operator
class BinaryOp : public Exp {
    public:
        BinaryOp(Exp* c1, BINOP _op, Exp* c2);
        ~BinaryOp();
        
        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        Exp* copy() override;
        Exp* substitute(std::string to_sub, std::string sub) override;
    
    private:
        BINOP op;
        Exp *child1, *child2;
};

// Integer value
class Integer : public Exp {
    public:
        Integer(int i);
        ~Integer() {}

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        Exp* copy() override;
        Exp* substitute(std::string to_sub, std::string sub) override;
        
    private:
        int val;
};

// Boolean value
class Bool : public Exp {
    public:
        Bool(bool b);
        ~Bool() {}

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        Exp* copy() override;
        Exp* substitute(std::string to_sub, std::string sub) override;
        
    private:
        bool val;
};

// Variable (currently only integer, should be easy to expand)
class Var : public Exp {
    public:
        Var(std::string _id);
        ~Var() {}

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        Exp* copy() override;
        Exp* substitute(std::string to_sub, std::string sub) override;
        
    private:
        std::string id;
};

#endif
