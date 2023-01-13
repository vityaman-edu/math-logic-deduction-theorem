#ifndef DEDUCTION_THEOREM_PARSE_H
#define DEDUCTION_THEOREM_PARSE_H

#include "expression.h"
#include <string>
#include <vector>

void parse_header(
    const std::string& src, 
    std::vector<expression*>& assumptions,
    expression*& result
);

expression* parse_expression(const std::string& src);

#endif
