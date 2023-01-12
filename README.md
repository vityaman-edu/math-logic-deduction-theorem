# math-logic-deduction-theorem

```c
bool is_axiom_1(expression* e) {
    // (1) a -> (b -> a)
    if (e->type() != IMPLICATION) return false;

    auto base_impl = e->as_binary();
    auto a = base_impl->left();
    auto right = base_impl->right();

    if (right->type() != IMPLICATION) return false;

    auto right_impl = right->as_binary();
    auto b = right_impl->left();
    auto a1 = right_impl->right();

    if (a->is_equal_to(a1)) return true;

    return false;
}

bool is_axiom(expression* e) {
    if (is_axiom_1(e)) return true;
    // 2
    // 3
    // 4
    // 5
    // 6
    // 7
    // 8
    // 9
    // 10
}
```