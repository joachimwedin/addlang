/* Definitions */
%{
#include <string>
#include "node.hpp"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

ws (\t|" ")+
digit [0-9]
num {digit}+

%% /* Rules */

{ws}               ;
"\n"               ;
{num}              SAVE_TOKEN; return INTEGER;
"+"                return TOKEN(PLUS);
.                  printf("Unknown token!n: %s", yytext); yyterminate();

%% /* User code */
