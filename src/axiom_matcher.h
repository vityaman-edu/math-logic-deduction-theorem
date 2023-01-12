#ifndef DEDUCTION_THEOREM_AXIOM_MATCHER_H
#define DEDUCTION_THEOREM_AXIOM_MATCHER_H

#include <unordered_map>
#include <vector>
#include "expression.h"

class axiom_matcher {
public:
    axiom_matcher() {
        place* a = p(1);
        place* b = p(2);
        place* c = p(3);
        axioms.emplace_back(
            // (1) a -> (b -> a)
            impl(a, impl(b, a))
        );
        axioms.emplace_back(
            // (2) (a -> b) -> ((a -> (b -> c)) -> (a -> c))
            impl(
                impl(a, b),
                impl(
                    impl(a, impl(b, c)),
                    impl(a, c)
                )
            )
        );
        axioms.emplace_back(
            // (3) a -> (b -> (a & b))
            impl(a, impl(b, conj(a, b)))
        );
        axioms.emplace_back(
            // (4) (a & b) -> a
            impl(conj(a, b), a)
        );
        axioms.emplace_back(
            // (5) (a & b) -> b
            impl(conj(a, b), b)
        );
        axioms.emplace_back(
            // (6) a -> (a | b)
            impl(a, disj(a, b))
        );
        axioms.emplace_back(
            // (7) b -> (a | b)
            impl(b, disj(a, b))
        );
        axioms.emplace_back(
            // (8) (a -> c) -> ((b -> c) -> ((a | b) -> c))
            impl(
                impl(a, c),
                impl(impl(b, c), impl(disj(a, b), c))
            )
        );
        axioms.emplace_back(
            // (9) (a -> b) -> ((a -> !b) -> !a)
            impl(
                impl(a, b),
                impl(impl(a, neg(b)), neg(a))
            )
        );
        axioms.emplace_back(
            // (10) !!a -> a
            impl(neg(neg(a)), a)
        );
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
    place* p(int id) {
        return new place(id, _table);
    }

    implication* impl(expression* l, expression* r) {
        return new implication(l, r);
    }

    conjunction* conj(expression* l, expression* r) {
        return new conjunction(l, r);
    }

    disjunction* disj(expression* l, expression* r) {
        return new disjunction(l, r);
    }

    negation* neg(expression* c) {
        return new negation(c);
    }

    std::vector<expression*> axioms;
    std::unordered_map<int, expression*> _table;
};

#endif
