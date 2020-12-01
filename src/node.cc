#include "node.h"
#include "exp.h"
#include <cstddef>
#include <memory>
#include <z3++.h>

#define TAB_SIZE 4

void printTree(Node* root) {
    root->print(0);
}

std::string gen_indent(size_t indent) {
    return std::string(indent * TAB_SIZE, ' ');
}

void printPredicate(Exp* predicate, size_t indent) {
    std::cout << gen_indent(indent) << "{ "
    << predicate->to_string() << " }" << std::endl;
}

void verifyTree(Node* root) {
    z3::context c;
    z3::solver s(c);

    z3::check_result res = dynamic_cast<Block*>(root)->verify(&c, &s); 

    switch (res) {
        case z3::unsat:
            std::cout << "Success" << std::endl; break;
        case z3::sat:
            std::cout << "Failed, counterexample:" << std::endl; 
            std::cout << s.get_model() << std::endl;
            break;
        case z3::unknown:
            std::cout << "Unknown" << std::endl; break;
    }
}

Node::Node() {

}


Block::Block(std::vector<Node*> c, std::vector<Exp*> p)
: commands(c), predicates(p) {}

void Block::print(size_t indent) {
    assert(commands.size() == predicates.size()-1);
    
    std::cout << gen_indent(indent) << "Block:" << std::endl;

    int i;
    for(i = 0; i < commands.size(); i++) {
        printPredicate(predicates[i], indent+1);
        commands[i]->print(indent+1);
    }
    printPredicate(predicates[i], indent+1);
}

// Verify block statement without pre and postconditions (root of program)
z3::check_result Block::verify(z3::context *c, z3::solver* s) {
    assert(commands.size() == predicates.size()-1);
    assert(predicates.size() > 0);
    
    z3::check_result res;

    for(int i = 0; i < commands.size(); i++) {
        Exp* pre = predicates[i];
        Node* com = commands[i];
        Exp* post = predicates[i+1];

        if((res = com->verify(pre, post, c, s)) != z3::unsat) 
            return res;
    }
    
    return res;
}


z3::check_result Block::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    assert(commands.size() == predicates.size()-1);
    assert(predicates.size() > 0);
    
    z3::check_result res;

    // Verify that preC and first predicate do not
    // contradict each other
    std::unique_ptr<Node> dummy_skip(new Skip);
    if((res = dummy_skip->verify(pre, predicates[0], c, s)) != z3::unsat)
        return res;

    // Loop over hoare triples, verifying each:
    // Block:   p1 c1 p2 c2 p3 c3 p4 ...
    // round1:  |------|
    // round2:        |------|
    // round3:              |------|
    // etc..
    for(int i = 0; i < commands.size(); i++) {
        Exp* pre = predicates[i];
        Node* com = commands[i];
        Exp* post = predicates[i+1];

        if((res = com->verify(pre, post, c, s)) != z3::unsat) 
            return res;
    }
    
    // Verify that last predicate and postC do not
    // contradict each other
    res = dummy_skip->verify(predicates.back(), post, c, s);
    
    // Only got here if everything before was unsat: 
    // simply return result of last check
    return res;
}


IfElse::IfElse(Exp* _bexp, Node* _if_body, Node* _else_body)
: bexp(_bexp), if_body(_if_body), else_body(_else_body) {}

void IfElse::print(size_t indent) {
    std::cout << gen_indent(indent) << "If " 
    << bexp->to_string() << " then" << std::endl;
    if_body->print(indent+1);
    
    std::cout << gen_indent(indent) << "else" << std::endl;
    else_body->print(indent+1);
}

z3::check_result IfElse::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    return z3::unknown;
}

While::While(Exp* _bexp, Node* _body) 
: bexp(_bexp), body(_body) {}

void While::print(size_t indent) {
    std::cout << gen_indent(indent) << "While " 
    << bexp->to_string() << " do" << std::endl;
    body->print(indent+1);
}

z3::check_result While::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    return z3::unknown;
}


Assign::Assign(Exp* _id, Exp* _aexp)
: id(_id), aexp(_aexp) {}

void Assign::print(size_t indent) {
    std::cout << gen_indent(indent) << id->to_string() 
    << " := " << aexp->to_string() << std::endl;
}

z3::check_result Assign::verify(Exp* pre, Exp* post, z3::context *c, z3::solver* s) {
    return z3::unknown;
}


Skip::Skip() {}

void Skip::print(size_t indent) {
    std::cout << gen_indent(indent) << "skip\n"; 
}

z3::check_result Skip::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    s->add(pre->to_Z3(c));
    
    s->push();
    s->add(!post->to_Z3(c));

    z3::check_result res = s->check();

    if(res == z3::unsat) s->pop();

    return res;
}

