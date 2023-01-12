%{
#include <iostream>
#include <string>
#include <vector>
#include "expression.h"

int yylex(void);
void yyerror(const char *);

std::vector<expression*> context;
std::vector<expression*> proof;
expression* result = nullptr;
%}

%union {
expression* e;
std::string* name;
}

%token <name> NAME
%token IMP OR AND NEG
%token LEFT RIGHT
%token END
%token COMMA
%token DEDUCE

%right IMP
%left OR
%left AND
%left NEG

%type <e> Input Expression

%start Input

%%
Input
    : Context DEDUCE Expression END Proof { result = $3; }
    ;

Context
    : Expression                   { context.push_back($1);    }
    | Expression COMMA Context     { context.push_back($1);    }
    ;

Proof
    : Expression END               { proof.push_back($1);      }
    | Expression END Proof         { proof.push_back($1);      }
    ;

Expression
    : Expression IMP Expression    { $$ = new implication($1, $3); }
    | Expression OR Expression     { $$ = new disjunction($1, $3); }
    | Expression AND Expression    { $$ = new conjunction($1, $3); }
    | NEG Expression               { $$ = new negation($2);        }
    | LEFT Expression RIGHT        { $$ = $2;                      }
    | NAME                         { $$ = new variable(*$1);       }
    ;

%%
