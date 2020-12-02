#include "node.h"
#include <memory>

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

