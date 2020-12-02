#ifndef exp_h
#define exp_h

#include <cstddef>
#include<z3++.h>

enum BINOP {
    OP_ADD, OP_SUB, OP_DIV, OP_MUL, // arithmetic
    OP_AND, OP_OR, // boolean
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LEQ, OP_GEQ // relative
};

enum UNOP {
    OP_NEG
};

class Exp {
    public:
        Exp();
        ~Exp();
        
        virtual std::string to_string() = 0;
        virtual z3::expr to_Z3(z3::context* c) = 0;
        virtual bool contains(std::string to_find) = 0;

    protected:
        std::string text;
};


class UnaryOp : public Exp {
    public:
        UnaryOp(UNOP _op, Exp* c);

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;

    private:
        UNOP op;
        Exp* child;
};

class BinaryOp : public Exp {
    public:
        BinaryOp(Exp* c1, BINOP _op, Exp* c2);
        
        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
    
    private:
        BINOP op;
        Exp *child1, *child2;
};

class Integer : public Exp {
    public:
        Integer(int i);

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        
    private:
        int val;
};

class Bool : public Exp {
    public:
        Bool(bool b);

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        
    private:
        bool val;
};

class Var : public Exp {
    public:
        Var(std::string _id);

        z3::expr to_Z3(z3::context* c) override;
        std::string to_string() override;
        bool contains(std::string to_find) override;
        
    private:
        std::string id;
};

#endif
