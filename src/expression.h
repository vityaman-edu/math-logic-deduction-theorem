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
    virtual bool is_equal_to(const expression& other) const = 0;
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

    bool is_equal_to(const expression& other) const override {
        if (other.type() != this->type()) {
            return false;
        }
        const binary_operation& that 
            = static_cast<const binary_operation&>(other);
        return _left->is_equal_to(*that._left)
            && _right->is_equal_to(*that._right);
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

    bool is_equal_to(const expression& other) const override {
        if (other.type() != this->type()) {
            return false;
        }
        const unary_operation& that 
            = static_cast<const unary_operation&>(other);
        return _child->is_equal_to(*that._child);
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

    bool is_equal_to(const expression& other) const override {
        if (other.type() != this->type()) {
            return false;
        }
        const variable& that 
            = static_cast<const variable&>(other);
        return _name == that._name;
    }

private:
    std::string _name;
};

#endif
