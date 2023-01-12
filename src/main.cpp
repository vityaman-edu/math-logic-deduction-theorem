#include "expression.h"
#include "parse/gen/expression.tab.hpp"
#include "parse/gen/expression.lexer.hpp"
#include <iostream>

extern expression* result;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

expression* input_expression() {
    std::string input;
    std::cin >> input;
    yy_scan_string(input.c_str());
    yyparse();
    return result;
}

int main() {
    const expression* a = input_expression();
    const expression* b = input_expression();

    std::cout 
        << a->is_equal_to(*b)
        << std::endl;

    return 0;
}