%{
#include <string>
#include "expression.h"
#include "parse/gen/expression.tab.hpp"
%}

%option outfile="lex.cpp" header-file="lex.hpp"

white [ \r\t\n]+
digit [0-9]
letter [A-Z]
suffix [`']
symbol {letter}({letter}|{digit}|{suffix})*

%%
{symbol} {
    yylval.name = new std::string(yytext);
    return NAME;
}
"&"  return AND;
"|"  return OR;
"->" return IMP;
"!"  return NEG;
"("  return LEFT;
")"  return RIGHT;
"\n" return END;
","  return COMMA;
"|-" return DEDUCE;
%%
