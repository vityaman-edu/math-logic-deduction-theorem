#ifndef EXPRESSION_TRUTHFULNESS_EXPRESSION_H
#define EXPRESSION_TRUTHFULNESS_EXPRESSION_H

#include <string>

class expression {
public:
    virtual std::string as_string() = 0;
    virtual ~expression() {};
};

class binary_operation : public expression {
public:
    binary_operation(
        const std::string& symbol,
        expression* left,
        expression* right
    ) : _symbol(symbol), 
        _left(left),
        _right(right) {}

    std::string as_string() override {
        return "(" 
            + _symbol + "," 
            + _left->as_string() + "," 
            + _right->as_string() 
            + ")";
    }

private:
    std::string _symbol;
    expression* _left;
    expression* _right;
};

class implication : public binary_operation {
public:
    implication(expression* left, expression* right) 
        : binary_operation("->", left, right) {}
};

class disjunction : public binary_operation {
public:
    disjunction(expression* left, expression* right) 
        : binary_operation("|", left, right) {}
};

class conjunction : public binary_operation {
public:
    conjunction(expression* left, expression* right) 
        : binary_operation("&", left, right) {}
};

class unary_operation : public expression {
public:
    unary_operation(std::string const & symbol, expression* child) 
        : _symbol(symbol),
          _child(child) {}

    inline std::string as_string() override {
        return "(" 
            + _symbol 
            + _child->as_string() 
            + ")";
    }

private:
    std::string _symbol;
    expression* _child;
};

class negation : public unary_operation {
public:
    negation(expression* child) 
        : unary_operation("!", child) {}
};

class variable : public expression {
public:
    variable(const std::string& name)
        : _name(name) {}

    inline std::string as_string() override {
        return _name;
    }

private:
    std::string _name;
};

#endif
