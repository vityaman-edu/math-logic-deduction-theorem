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

int main() {
    std::string input;
    std::cin >> input;
    yy_scan_string(input.c_str());
    yyparse();

    std::cout 
        << result->as_string()
        << std::endl;

    return 0;
}