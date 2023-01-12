#ifndef DEDUCTION_THEOREM_AXIOM_MATCHER_H
#define DEDUCTION_THEOREM_AXIOM_MATCHER_H

#include <unordered_map>
#include <vector>
#include "expression.h"

class axiom_matcher {
public:
    axiom_matcher() {
        axioms.emplace_back(_impl(_place(1), _impl(_place(2), _place(1))));
    }

    bool is_axiom(expression* expression) {
        for (auto& axiom : axioms) {
            _table.clear();
            if (axiom->is_equal_to(expression)) {
                return true;
            }
        }
        return false;
    }

private:
    place* _place(int id) {
        return new place(id, _table);
    }

    implication* _impl(expression* l, expression* r) {
        return new implication(l, r);
    }

    std::vector<expression*> axioms;
    std::unordered_map<int, expression*> _table;
};

#endif
