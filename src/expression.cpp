#include "expression.h"
#include <string>
#include <unordered_map>

expression_type expression::type() {
    return _type;
}   

binary_operation* expression::as_binary() {
    return static_cast<binary_operation*>(this);
}

unary_operation* expression::as_unary() {
    return static_cast<unary_operation*>(this);
}
