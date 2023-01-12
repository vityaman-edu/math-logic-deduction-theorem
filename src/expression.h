#ifndef EXPRESSION_TRUTHFULNESS_EXPRESSION_H
#define EXPRESSION_TRUTHFULNESS_EXPRESSION_H

#include <string>
#include <unordered_map>

enum expression_type {
    IMPLICATION,
    DISJUNCTION,
    CONJUNCTION,
    NEGATION,
    VARIABLE,
    PLACE
};

class binary_operation;
class unary_operation;

class expression {
public:
    expression(expression_type type)
        : _type(type) {}

    expression_type type();
    binary_operation* as_binary();
    unary_operation* as_unary();

    virtual std::string as_string() = 0;
    virtual bool is_equal_to(expression* other) = 0;
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
            + _left->as_string() 
            + _symbol
            + _right->as_string() 
            + ")";
    }

    bool is_equal_to(expression* other) override {
        if (other->type() != type()) {
            return false;
        }
        binary_operation* that 
            = static_cast<binary_operation*>(other);
        return _left->is_equal_to(that->_left)
            && _right->is_equal_to(that->_right);
    }

    expression* left() {
        return _left;
    }

    expression* right() {
        return _right;
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
        const std::string & symbol, 
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

    bool is_equal_to(expression* other) override {
        if (other->type() != type()) {
            return false;
        }
        unary_operation* that 
            = static_cast<unary_operation*>(other);
        return _child->is_equal_to(that->_child);
    }

    expression* child() {
        return _child;
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

    bool is_equal_to(expression* other) override {
        if (other->type() != type()) {
            return false;
        }
        variable* that 
            = static_cast<variable*>(other);
        return _name == that->_name;
    }

private:
    std::string _name;
};

class place : public expression {
public:
    place(int id, std::unordered_map<int, expression*>& table)
        : expression(PLACE),
          _id(id), 
          _table(&table) {}

    inline std::string as_string() override {
        auto e = _table->find(_id);
        if (e == _table->end()) {
            return "$";
        }
        return e->second->as_string();
    }

    bool is_equal_to(expression* other) override {
        auto e = _table->find(_id);
        if (e == _table->end()) {
            _table->insert({ _id, other });
            return true;
        }
        return e->second->is_equal_to(other);
    }
    
private:
    int _id;
    std::unordered_map<int, expression*>* _table;
};

#endif
