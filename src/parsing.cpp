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

// Your f: String -> Expression
expression* parse_expression(const std::string& src) {
    yy_scan_string(src.c_str());
    yyparse();
    return result;
}

/*
    E1, E2, E3, E4, Alpha |- Result
    ...
    class Header {
        Collection<Expression> assumptions;
        Expression result;

        static Header parse(String text) {
            "  E1, E2,  E3, E4, Alpha  " |- " Result  "
            String[] assumptionsAndResult = text.split("|-");
            String assumptionsString = assumptionsAndResult[0];
            String resultString = assumptionsAndResult[1];
            String[] assumptionStrings = assumptionsString.split(",");

            List<Expression> assumptions = new ArrayList<>();
            for (String assumptionString : assumptionStrings) {
                assumptions.add(parseExpression(assumptionString));
            }

            Expression result = parseExpression(resultString);

            return Header(assumptions, result);
        }
    }
*/
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


;