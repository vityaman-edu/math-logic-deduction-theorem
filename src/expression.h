#ifndef DEDUCTION_THEOREM_EXPRESSION_H
#define DEDUCTION_THEOREM_EXPRESSION_H

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

    // don't need
    expression_type type();
    binary_operation* as_binary();
    unary_operation* as_unary();

    virtual std::string as_string() = 0;

    // A -> A
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
            + _left->as_string() // A & B
            + _symbol // &, |, ->
            + _right->as_string() // C
            + ")"; // (A & B) -> (C)
    }

    /*
        class And extends Expression {
            ...

            public String asString() {
                return "("
                    + left.asString()
                    + " & "
                    + right.asString()
                    + ")";
            }

            ...

            public bool mathes(Expression expression) {
                if (expression.getClass() != getClass()) {
                    return false;
                }
                And that = (And) expression;
                return this.left.mathes(that.left)
                    && this.right.mathes(that.right);
            }

            ...
        }
    */
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

    /*
        class Variable extends Expression {
            ...

            public String asString() {
                return this.name;
            }

            // (A -> A) != (B -> B) != ((A -> A) -> (A -> A))
            // we want sometimes 
            //   (A -> A).matches(((A -> A) -> (A -> A)))
            //   == true in case of (A -> A) is axiom
            public bool matches(Expression other) {
                if (expression.getClass() != getClass()) {
                    return false;
                }
                Variable that = (Variable) other;
                return this.name.equals(other.name);
            }

            ...
        }
    */
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
    /*
        class Table {
             = MAX_AXIOM_VAR_COUNT
            Expression[] table = new Expression[N];
            table[i] == null initially

            bool contains(int i) {
                return table[i] != null;
            }

            Expression get(int i) {
                return table[i]; // nullable
            }

            void set(int i, Expression e) {
                table[i] = e;
            }

            void clear() {
                for (int i = 0; i < N; i++) {
                    table[i] = null;
                }
            }
        }

        class Place {
            Table table;
            int id;
        }
    */
    place(int id, std::unordered_map<int, expression*>& table)
        : expression(PLACE),
          _id(id), 
          _table(&table) {}

    /*
        String asString() {
            if (table.contains(id)) {
                return table.get(id).asString();
            } else {
                return "$";
            }
        }
    */
    inline std::string as_string() override {
        auto e = _table->find(_id);
        if (e == _table->end()) {
            return "$";
        }
        return e->second->as_string();
    }

    /*
        bool matches(Expression other) {
            if (table.contains(id)) {
                return table.get(id).matches(other);
            } else {
                table.set(id, other);
                return true;
            }
        }
    */
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
