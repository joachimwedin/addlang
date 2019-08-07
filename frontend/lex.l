/* Definitions */
%{
#include <string>
#include "node.hpp"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%% /* Rules */

[ \t\n]            ;
[0-9]+             SAVE_TOKEN; return INTEGER;
"+"                return TOKEN(PLUS);
.                  printf("Unknown token!n"); yyterminate();

%% /* User code */
