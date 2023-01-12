#include "expression.h"
#include "parse/gen/expression.tab.hpp"
#include "parse/gen/expression.lexer.hpp"
#include <iostream>
#include <algorithm> 
#include <vector>

extern std::vector<expression*> context;
extern std::vector<expression*> proof;
extern expression* result;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

void read_input() {
    std::string input = 
    "A, (A & B), (C | M) |- (A -> D) \n"
    "A \n"
    "LALA \n"
    "LEND -> A\n";
    // std::cin >> input;

    yy_scan_string(input.c_str());
    yyparse();

    std::reverse(context.begin(), context.end());
    std::reverse(proof.begin(), proof.end());
}

int main() {
    read_input();

    std::cout << "Context:" << std::endl;
    for (auto& expr : context) {
        std::cout << expr->as_string() << std::endl;
    }

    std::cout << "Result: " << result->as_string() << std::endl;

    std::cout << "Proof:" << std::endl;
    for (auto& expr : proof) {
        std::cout << expr->as_string() << std::endl;
    }

    return 0;
}