#include "node.h"
#include <cstddef>
#include <memory>
#include <list>
#include <iomanip>
#include <string>

Block::Block(std::vector<Node*> c, std::vector<Exp*> p, size_t _line)
: Node(_line), commands(c), predicates(p) {
    assert(p.size() == c.size() - 1 || c.size() == p.size() - 1);
}

Block::~Block() {
    for(auto &com : commands) {
        delete com;
    }
    for(auto &pred : predicates) {
        delete pred;
    }
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

void Block::log() {
    std::cout << std::setw(LOG_WIDTH) << std::left << 
    "Block with " + std::to_string(commands.size()) + " commands";
}

// Verify block statement without pre and postconditions (root of program)
Result Block::verify(z3::context *c, z3::solver* s) {
    assert(predicates.size() > 0);
    
    Result res(s);

    // Loop over hoare triples, verifying each:
    // Block:   p1 c1 p2 c2 p3 c3 p4 ...
    // round1:  |------|
    // round2:        |------|
    // round3:              |------|
    for(size_t i = 0; i < commands.size(); i++) {
        res = commands[i]->verify(
            predicates[i], predicates[i+1], c, s
        );
        if(!(res = commands[i]->verify(
            predicates[i], predicates[i+1], c, s
        )).valid()) {
            return res.log(this);
        }
    }
    
    return res;
}


Result Block::verify(
    Exp* pre, Exp* post, z3::context *c, z3::solver* s
) {
    assert(commands.size() > 0);
    
    Result res(s);
    
    // Extend predicates: pre . predicates . post
    std::list<Exp*> preds(predicates.begin(), predicates.end());
    preds.push_front(pre);
    preds.push_back(post);

    // If this is not a while block, also add dummy skips: skip . commands . skip
    std::list<Node*> coms(commands.begin(), commands.end());
    std::unique_ptr<Dummy> dummy(new Dummy);
    if(predicates.size() > commands.size()) {
        coms.push_front(dummy.get());
        coms.push_back(dummy.get());
    }

    // Check all triples
    auto pred_it = preds.begin();
    for(auto com_it = coms.begin(); com_it != coms.end(); com_it++) {
        auto _pre = *pred_it;
        auto _post = *(++pred_it);

        if(!(res = (*com_it)->verify(_pre, _post, c, s)).valid()) {
            return res.log(this);
        }
    }
    
    // Only got here if everything before was unsat: 
    // simply return result of last check
    return res;
}

