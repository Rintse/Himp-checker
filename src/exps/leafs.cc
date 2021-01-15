// Implementations of the various AST leaf nodes
#include "exp.h"

// Integers
Integer::Integer(int i) : val(i) {}

z3::expr Integer::to_Z3(z3::context* c) {
    return c->int_val(val);
}

std::string Integer::to_string() {
    return std::to_string(val);
}

bool Integer::contains(std::string to_find) {
    return false;
}

Exp* Integer::copy() {
    return new Integer(val);
}

Exp* Integer::substitute(std::string to_sub, std::string sub) {
    return copy();
}


// Bools
Bool::Bool(bool b) : val(b) {}

z3::expr Bool::to_Z3(z3::context *c) {
    return c->bool_val(val);
}

std::string Bool::to_string() {
    return val ? "true" : "false";
}

bool Bool::contains(std::string to_find) {
    return false;
}

Exp* Bool::copy() {
    return new Bool(val);
}

Exp* Bool::substitute(std::string to_find, std::string replacement) {
    return copy();
}


// Variables
Var::Var(std::string _id) : id(_id) {}

z3::expr Var::to_Z3(z3::context *c) {
    return c->int_const(id.c_str());
}

std::string Var::to_string() {
    return id;
}

bool Var::contains(std::string to_find) {
    return id == to_find;
}

Exp* Var::copy() {
    return new Var(id);
}

Exp* Var::substitute(std::string to_find, std::string replacement) {
    return new Var(id == to_find ? replacement : id);
}

