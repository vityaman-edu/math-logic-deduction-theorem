#ifndef EXPRESSION_TRUTHFULNESS_EXPRESSION_H
#define EXPRESSION_TRUTHFULNESS_EXPRESSION_H

#include <string>

enum expression_type {
    IMPLICATION,
    DISJUNCTION,
    CONJUNCTION,
    NEGATION,
    VARIABLE
};

class expression {
public:
    expression(expression_type type)
        : _type(type) {}

    expression_type type() const {
        return _type;
    }

    virtual std::string as_string() = 0;
    virtual ~expression() {};

private:
    expression_type _type;
};

class binary_operation : public expression {
public:
    binary_operation(
        const std::string& symbol,
        expression_type type,
        expression* left,
        expression* right
    ) : expression(type), 
        _symbol(symbol), 
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
        : binary_operation("->", IMPLICATION, left, right) {}
};

class disjunction : public binary_operation {
public:
    disjunction(expression* left, expression* right) 
        : binary_operation("|", DISJUNCTION, left, right) {}
};

class conjunction : public binary_operation {
public:
    conjunction(expression* left, expression* right) 
        : binary_operation("&", CONJUNCTION, left, right) {}
};

class unary_operation : public expression {
public:
    unary_operation(
        std::string const & symbol, 
        expression_type type,
        expression* child
    ) : expression(type),
        _symbol(symbol),
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
        : unary_operation("!", NEGATION, child) {}
};

class variable : public expression {
public:
    variable(const std::string& name)
        : expression(VARIABLE),
          _name(name) {}

    inline std::string as_string() override {
        return _name;
    }

private:
    std::string _name;
};

#endif
