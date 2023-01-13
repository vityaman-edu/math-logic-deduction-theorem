#include "expression.h"
#include "parse/gen/expression.tab.hpp"
#include "parse/gen/expression.lexer.hpp"
#include "parsing.h"

#include <iostream>
#include <sstream> 
#include <vector>

extern expression* result;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

expression* parse_expression(const std::string& src) {
    yy_scan_string(src.c_str());
    yyparse();
    return result;
}

void parse_header(
    const std::string& src, 
    std::vector<expression*>& assumptions,
    expression*& res
) {
    std::stringstream stream(src);
    while (stream.good()) {
        std::string expr;
        getline(stream, expr, ',');
        size_t deduce_pos = expr.find("|-");
        if (deduce_pos == std::string::npos) {
            assumptions.push_back(parse_expression(expr));    
        } else {
            assumptions.push_back(parse_expression(expr.substr(0, deduce_pos)));
            res = parse_expression(expr.substr(deduce_pos + 2));
        }
    }
}


