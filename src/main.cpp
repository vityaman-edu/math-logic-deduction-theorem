#include "expression.h"
#include "parsing.h"
#include "axiom_matcher.h"
#include "parse/gen/expression.tab.hpp"
#include "parse/gen/expression.lexer.hpp"
#include <iostream>
#include <algorithm> 
#include <vector>

extern std::vector<expression*> context;
extern std::vector<expression*> proof;
expression* res;

bool is_blank(const std::string& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (!std::isspace(s[i])) {
            return false;
        }
    }
    return true;
}

void read_input() {
    std::string line;
    std::getline(std::cin, line);
    parse_header(line, context, res);
    while (std::getline(std::cin, line)) {
        if (!is_blank(line)) {
            proof.push_back(parse_expression(line));
        }
    }
}

void case_is_axiom(const std::string& st, const std::string& a) {
    std::cout 
        << st + "->(" + a + "->" + st + ")" 
        << std::endl;
    std::cout 
        << "(" + st + ")"
        << std::endl;
}

void case_is_equal_to_a(const std::string& st, const std::string& a) {
    const std::string a_impl_a = "(" + a + "->" + a + ")";
    std::cout 
        << a + "->" + a_impl_a
        << std::endl;
    std::cout
        << "(" + a + "->" + a_impl_a +")"
         + "->"
         + "(" + a + "->(" + a_impl_a + "->" + a + "))"
         + "->" + a_impl_a
        << std::endl;
    std::cout 
        << "(" + a + "->" + a_impl_a + "->" + a + ")->" + a_impl_a
        << std::endl;
    std::cout 
        << a + "->" + a_impl_a + "->" + a
        << std::endl;
}

int main() {
    // freopen("res/output.txt", "w", stdout);
    // freopen("res/input.txt",  "r", stdin );

    read_input();

    expression* alpha = context[context.size() - 1];

    axiom_matcher matcher;

    for (size_t i = 0; i < context.size() - 1; ++i) {
        matcher.add_assumption(context[i]);
    }

    std::string alpha_str = alpha->as_string();

    for (int i = 0; i < (int)context.size() - 2; ++i) {
        std::cout << context[i]->as_string() << ",";
    }
    if (context.size() > 1) {
        std::cout << context[context.size() - 2]->as_string();
    }
    std::cout
        << "|-(" + alpha_str + "->(" + res->as_string() + "))"
        << std::endl;

    std::string a = alpha_str;
    for (size_t i = 0; i < proof.size(); ++i) {
        auto statement = proof[i];
        std::string st = statement->as_string();
        if (matcher.is_axiom(statement)) {
            case_is_axiom(st, a);
        } else if (statement->is_equal_to(alpha)) {
            case_is_equal_to_a(st, a);
        } else {
            std::vector<implication*> impls;
            for (size_t j = 0; j < i; ++j) {
                if (proof[j]->type() == IMPLICATION) {
                    auto impl = static_cast<implication*>(proof[j]);
                    if (impl->right()->is_equal_to(statement)) {
                        impls.push_back(impl);
                    }
                }
            }

            expression* left = new variable("A");
            for (size_t k = 0; k < impls.size(); ++k) {
                for (size_t j = 0; j < i; ++j) {
                    if (impls[k]->left()->is_equal_to(proof[j])) {
                        left = proof[j];
                        break;
                    }
                }
            }
            
            std::string l = left->as_string();
            // (a -> l) -> ((a -> (l -> st)) -> (a -> st))
            std::cout
                << "(" + a + "->" + l + ")" 
                + "->((" + a + "->(" + l + "->" + st + "))"
                + "->(" + a + "->" + st + "))"
                << std::endl;
            // (a -> (l -> st)) -> (a -> st)
            std::cout 
                << "(" + a + "->(" + l + "->" + st + "))"
                + "->(" + a + "->" + st + ")"
                << std::endl;
        }
        std::cout 
            << "(" + alpha_str << "->" << st  + ")" 
            << std::endl;
    }

    return 0;
}