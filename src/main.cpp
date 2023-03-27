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

/*
    E1
            <-- empty string, don't forget to check
    E2  
*/
bool is_blank(const std::string& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (!std::isspace(s[i])) {
            return false;
        }
    }
    return true;
}

/*
    class Input {
        Header header;
        List<Expression> steps; // proof

        Input read() {
            ...
        }
    }
*/
void read_input() {
    std::string line;
    std::getline(std::cin, line);
    // context - header.assumptions
    // res - header.result
    parse_header(line, context, res); 
    while (std::getline(std::cin, line)) {
        if (!is_blank(line)) {
            // proof - input.steps
            proof.push_back(parse_expression(line));
        }
    }
}

void case_is_axiom(const std::string& st, const std::string& a) {
    // Note: 
    //   System.out.print("Hello, ");
    //   System.out.println("World!");
    // is slower than
    //   System.out.println("Hello, " + "World!");
    // because of IO performance

    // System.out.println(st + "->(" + a + "->" + st + ")")
    std::cout 
        << st + "->(" + a + "->" + st + ")" 
        << std::endl;
    std::cout 
        << "(" + st + ")"
        << std::endl;
}

void case_is_equal_to_a(const std::string& st, const std::string& a) {
    const std::string a_impl_a = "(" + a + "->" + a + ")"; // cache
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

    // input = Input.read();
    read_input();

    // alpha = input.assumptions[input.assumptions.size() - 1];
    expression* alpha = context[context.size() - 1];

    // create AxiomMatcher and add assumptions
    axiom_matcher matcher;
    for (size_t i = 0; i < context.size() - 1; ++i) {
        matcher.add_assumption(context[i]);
    }

    // cache alpha.asString()
    std::string alpha_str = alpha->as_string();

    // output:
    // E1, ..., EN |- ($alpha -> $result)
    // print
    for (int i = 0; i < (int)context.size() - 2; ++i) {
        std::cout << context[i]->as_string() << ",";
    }
    if (context.size() > 1) { // trailing comma defence
        std::cout << context[context.size() - 2]->as_string();
    }
    // ... |- ($alpha -> $result)
    std::cout
        << "|-(" + alpha_str + "->(" + res->as_string() + "))"
        << std::endl;

    // std::string ~ java.util.String
    // std::cout << "Hello" << "lalaa" << std::endl
    //     ~ System.out.prinln("Hello" + "lalaa");
    std::string a = alpha_str; // short name

    // foreach step in Input.steps
    for (size_t i = 0; i < proof.size(); ++i) {
        auto statement = proof[i];
        std::string st = statement->as_string(); // cache

        if (matcher.is_axiom(statement)) {

            case_is_axiom(st, a);

        } else if (statement->is_equal_to(alpha)) {

            case_is_equal_to_a(st, a);

        } else {

            // case is Modus Ponens

            /*
                // maybe it works

                Expression statement = ...; // current statement delta_i
                ...
                // check proof on stepik
                Expression impl = null;                
                for (int k = i - 1; k >= 0; k--) {
                    Expression step = input.steps[k]; // .get(k)
                    if (step instanceof Implication) {
                        Expression stepAsImpl = (Implication) step;
                        if (stepAsImpl.matches(statement)) {
                            impl = step;
                            break;
                        }
                    }
                }

                // we have
                //  impl.right is current `statement` delta_i
                //  impl.left is delta_j
            */

            // we have `delta_i`
            // collect all statements in proof
            // such that they are (delta_j) -> (delta_i)
            std::vector<implication*> impls;
            for (size_t j = 0; j < i; ++j) {
                if (proof[j]->type() == IMPLICATION) {
                    auto impl = static_cast<implication*>(proof[j]);
                    // if (stepAsImpl.matches(statement)) {
                    if (impl->right()->is_equal_to(statement)) {
                        impls.push_back(impl);
                    }
                }
            }

            // now we have all that impls
            // find one `delta_j` from impls lefts
            // such that `delta_j` in proof
            expression* left = new variable("A");
            for (size_t k = 0; k < impls.size(); ++k) {
                for (size_t j = 0; j < i; ++j) {
                    if (impls[k]->left()->is_equal_to(proof[j])) {
                        left = proof[j];
                        break;
                    }
                }
            }
            

            // Print out
            // left - impl.left
            // st - statement.asString() cache

            std::string l = left->as_string(); // cache
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