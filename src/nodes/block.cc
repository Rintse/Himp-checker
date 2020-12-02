#include "node.h"
#include <cstddef>
#include <memory>
#include <list>

Block::Block(std::vector<Node*> c, std::vector<Exp*> p)
: commands(c), predicates(p) {
    assert(p.size() == c.size() - 1 || c.size() == p.size() - 1);
}

void Block::print(size_t indent) {
    std::cout << gen_indent(indent) << "Block:" << std::endl;
    size_t i;

    if(commands.size() > predicates.size()) {
        for(i = 0; i < predicates.size(); i++) {
            commands[i]->print(indent+1);
            printPredicate(predicates[i], indent+1);
        }
        commands[i]->print(indent+1);
    } else {
        for(i = 0; i < commands.size(); i++) {
            printPredicate(predicates[i], indent+1);
            commands[i]->print(indent+1);
        }
        printPredicate(predicates[i], indent+1);
    }
}

// Verify block statement without pre and postconditions (root of program)
z3::check_result Block::verify(z3::context *c, z3::solver* s) {
    assert(predicates.size() > 0);
    
    z3::check_result res;

    // Loop over hoare triples, verifying each:
    // Block:   p1 c1 p2 c2 p3 c3 p4 ...
    // round1:  |------|
    // round2:        |------|
    // round3:              |------|
    for(size_t i = 0; i < commands.size(); i++) {
        if((res = commands[i]->verify(
            predicates[i], predicates[i+1], c, s)
        ) != z3::unsat) {
            return res;
        }
    }
    
    return res;
}


z3::check_result Block::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    assert(commands.size() > 0);
    
    z3::check_result res;

    std::list<Exp*> preds(predicates.begin(), predicates.end());
    preds.push_front(pre);
    preds.push_back(post);

    std::list<Node*> coms(commands.begin(), commands.end());
    std::unique_ptr<Node> dummy_skip(new Skip);
    if(predicates.size() > commands.size()) {
        coms.push_front(dummy_skip.get());
        coms.push_back(dummy_skip.get());
    }

    // Check all triples
    auto pred_it = preds.begin();
    for(auto com_it = coms.begin(); com_it != coms.end(); com_it++) {
        auto _pre = *pred_it;
        auto _post = *(++pred_it);

        if((res = (*com_it)->verify(_pre, _post, c, s)) != z3::unsat)
            return res;
       
    }
    
    // Only got here if everything before was unsat: 
    // simply return result of last check
    return res;
}
